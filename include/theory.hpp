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
            curNode.board=node.board;
            if (curNode.board.at(x).g == 255) curNode.board.at(x) = {255,251,0};
            for (int c=0; c < curNode.board.size(); c++){
                if (curNode.board.at(c).r+curNode.board.at(c).b+curNode.board.at(c).g != 255*3 && c+collums < board.size() && curNode.board.at(c+collums).r+curNode.board.at(c+collums).b+curNode.board.at(c+collums).g == 255*3){
                    curNode.board.at(c+collums) = curNode.board.at(c); curNode.board.at(c) = {255,255,255};
                }
            }
        }
        node=*node.children.at(0);
        curNode=node;
    }
    return 0;
}