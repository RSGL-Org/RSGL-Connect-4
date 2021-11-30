#define fmax 3 //how many moves should it look ahead?

struct boardTree{
    boardTree* parent;
    std::vector<boardTree*> children;
    int value;
};


int aiChoice(std::vector<RSGL::color> board){
    boardTree tree = {NULL,{},0};
    for (int y=0; y < fmax; y++){
        for (int x=0; x < collums; x++){
            
        }
    }
    return 0;
}