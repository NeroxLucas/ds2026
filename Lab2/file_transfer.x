struct FileChunk {
    string filename<256>;
    opaque data<8192>;
    int size;
    int last;
};

program FILETRANSFER_PROG {
    version FILETRANSFER_VERS {
        int send_chunk(FileChunk) = 1;
    } = 1;
} = 0x31234567;
