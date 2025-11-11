// shouldn't be complied

void Map::update(Life &rules)
{
    // create buffer for chunks
    std::unordered_map<long long, Chunk> new_chunk;

    for (auto &[key, chunk] : chunks)
    {
        // list of relevant chunks
        std::vector<Chunk*> relevant_chunks;
        relevant_chunks.push_back(&chunk);
        // checking the chunk and its neighbors
        for (auto neighbor_key : get_neighbor_key(key))
        {
            // world contains any keys 
            if (chunks.contains(neighbor_key))
            {
                relevant_chunks.push_back(&chunks[neighbor_key]);
            }
        }
        // michael wright code fo game of life here
        Chunk update_chunk = rules.step(chunk, relevant_chunks);

        // store update chunk
        new_chunk[key] = std::move(update_chunk);
    }
    chunks = std::move(new_chunk);
}


// Life 
Chunk Life::step(Chunk chunk, std::vector<Chunk *> relevent_chunks)
{
    bool top = false;
    bool bottom = false;
    bool left = false;
    bool right = false;

    for (auto neighbor : relevent_chunks)
    {
        if (!neighbor)
            continue;

        // Check top edge
        for (int col = 0; col < Chunk::CHUNK_SIZE; ++col)
            if (neighbor->get_cell(0, col).is_alive())
                top = true;

        // Check bottom edge
        for (int col = 0; col < Chunk::CHUNK_SIZE; ++col)
            if (neighbor->get_cell(Chunk::CHUNK_SIZE - 1, col).is_alive())
                bottom = true;

        // Check left edge
        for (int row = 0; row < Chunk::CHUNK_SIZE; ++row)
            if (neighbor->get_cell(row, 0).is_alive())
                left = true;

        // Check right edge
        for (int row = 0; row < Chunk::CHUNK_SIZE; ++row)
            if (neighbor->get_cell(row, Chunk::CHUNK_SIZE - 1).is_alive())
                right = true;
    }

    // Now you know which edges have active neighbors
    // You can use this info to update `chunk`'s border cells

    std::cout << "alive Cells in";
    if (top)
    {
        std::cout << "top";
    }

    if (bottom)
    {
        std::cout << "bottom";
    }

    if (left)
    {
        std::cout << "left";
    }

    if (right)
    {
        std::cout << "right";
    }

    std::cout << '\n';

    return chunk;
};