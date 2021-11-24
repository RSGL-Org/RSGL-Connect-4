#define fmax 3 //how many moves should it look ahead?

struct boardTree{
    std::vector<RSGL::color> board;
    int value; 
    boardTree* parent;
    std::vector<boardTree*> children;
};


int aiChoice(std::vector<RSGL::color> board){
    boardTree tree = {board,0,NULL};
    boardTree node = tree; 
    boardTree curNode=node;
    for (int y=0; y < fmax; y++){
        for (int x=0; x < collums; x++){
            node.children.at(x) = {};
            curNode=*node.children.at(x);
        }
        node=*node.children.at(0);
        curNode=node;
    }
    return 0;
}