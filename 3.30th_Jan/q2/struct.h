struct Req {
    char type;
    char name[50];
    char id;
    char eof;
};
struct Reply {
    int id;
    char name[50];
    char eof;
};

struct Person {
    int id;
    char name[50];
};