
template<class T> int cmpDef(const T& a, const T& b)
{
    if(a < b)
        return -1;
    if(a > b)
        return 1;
    return 0;
}

template<class Key, class Val> class TTree
{
public:

};