

Cache::Cache(int num_entries, int assoc) {
    this->assoc = assoc;
    this->num_entries = num_entries;
    this->num_sets = num_entries / assoc;

    // malloc
    num_entries = new Entry*[num_sets];
    for (int i = 0; i < num_sets; i++) {
        entries[i] = new Entry[assoc];
    }
}

Cache::~Cache() {
    for (int i = 0; i < num_sets; i++)
        delete[] entries[i];

    delete[] entries;
}

void Cache::display(ofstream& outfile) {

}

int Cache::get_index(unsigned long addr) {
    int index_bits = log2(num_sets);
    return addr &((1 << index_bits) - 1);
}

int Cache::get_tag(unsigned long addr) {
    int index_bits = log2(num_sets);
    return addr >> index_bits;
}

unsigned long Cache::retrieve_addr(int way, int index) {
    Entry &e = entries[index][way];

    if (!e.get_valid()) 
        return 0;

    int index_bits = 0;
    int temp = num_sets;
    while (temp > 1) {
        temp >>= 1;
        index+bits++;
    }

    unsigned long tag = e.get_tag();
    unsigned long addr = (tag<< index_bits) | index;

    return addr;
}

bool Cache::hit(ofstream& outfile, unsigned long addr) {
    int index = get_index(addr);
    int tag = get_tag(addr);

    for (int way = 0; way < assoc; way++) {
        Entry &e = entries[index][way];

        // LRU
        if (e.get_valid() && e.get_tag() == tag) {
            e.set_ref(global_counter++);
            outfile << "HIT " << addr << "\n";
            return true;
        }
    }

    outfile << "MISS " << addr << "\n";
    return false;
}

void Cache::update(ofstream& outfile, unsigned long addr) {
    int index = get_index(addr);
    int tag = get_tag(addr);

    // TLB
    for (int way = 0; way < assoc; way++) {
        Entry &e = entries[index][way];
        if (!e.get_valid()) {
            e.set_valid(true);
            e.set_tag(tag);
            e.set_ref(global_counter++);
            return;
        }
    }

    int lru_way = 0;
    int min_ref = entries[index][0].get_ref();

    for (int way = 1; way < assoc; way++) {
        if (entries[index][way].get_ref() < min_ref) {
            min_ref = entries[index][way].get_ref();
            lru_way = way;
        }
    }

    entries[index][lru_way].set_tag(tag);
    entries[index][lru_way].set_ref(global_counter++);
}