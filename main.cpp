#include <iostream>
#include <stdlib.h>
using namespace std;
#define rep(n) for(int i=0;i<(n);i++)
#define jrep(n) for(int j=0;j<(n);j++)

// globals
int fen[76527504+6],arr[76527504+5];
char decc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
struct Slice{ uint8_t size; char* data; };
struct LL{ Slice *data; Slice *key; };
LL *hasht[76527504+5];
// end globals

// Linked List
bool eq(Slice *a, Slice *b){
    if(a->size!=b->size) return false;
    rep(a->size){
        if(a->data[i]!=b->data[i]) return false;
    } return true;
}

void gv(int index){
    rep(arr[index]) cout<<hasht[index][i].key->data<<" "; cout<<endl;
}

bool les(LL a, LL b){
    int n=a.key->size; if(n<(b.key->size)) n=b.key->size;
    rep(n){
        if((a.key->data[i])<(b.key->data[i])) return true;    
        else if((a.key->data[i])>(b.key->data[i])) return false;    
    }    
    return a.key->size < b.key->size;
}

void ins(int ind, Slice *d, Slice *k){
    arr[ind]++;
    hasht[ind] = (LL*) realloc(hasht[ind],arr[ind]*sizeof(LL));
    hasht[ind][arr[ind]-1].key = k; hasht[ind][arr[ind]-1].data = d;
    for(int i=arr[ind]-2;i>=0;i--) if(les(hasht[ind][i+1],hasht[ind][i])){
        LL tmp = hasht[ind][i+1]; hasht[ind][i+1]=hasht[ind][i]; hasht[ind][i]=tmp;
    }
}

bool del(int ind,Slice *k){
    bool found=false;
    rep(arr[ind]){
        if(eq(hasht[ind][i].key,k)) found=true;
        if(found) hasht[ind][i]=hasht[ind][i+1];
    }
    if(found) arr[ind]--,hasht[ind] = (LL*) realloc(hasht[ind],arr[ind]*sizeof(LL));
    return found;
}

Slice *get(int ind,Slice *k){
    rep(arr[ind]){ 
        if(eq(hasht[ind][i].key,k)) return hasht[ind][i].data;    
    } return NULL;
}
// end link list

// fen_tree

void addf(int index){
    index++;
    while(index<=76527504) fen[index]++,index+=(index&-index);
}

void delf(int index){
    index++;
    while(index<=76527504) fen[index]--,index+=(index&-index);
}

int query(int index){ index++; int su=0;
    while(index>0)
        su+=fen[index],index-=(index&-index);
    return su;
}

int gi(int index){
    rep(10000) if(query(i)>=index) return i; return -1;
}

// end_fen_tree

int main(){
    //rep(10){
    //    Slice *d = new Slice, *k = new Slice; 
    //    d->data = new char[64]; k->data = new char[255];
    //    d->size = 10; k->size=10;
    //    cin>>d->data>>k->data;
    //    ins(0,d,k);
    //    gv(0);
    //    //cout<<d.data<<" "<<k.data<<endl;
    //}
    //while(true){
    //    Slice *d = new Slice, *k = new Slice; 
    //    d->data = new char[64]; k->data = new char[255];
    //    d->size = 10; k->size=10;
    //    cin>>k->data;
    //    Slice *a = get(0,k);
    //    if(a!=NULL) cout<<a->data<<endl;
    //    del(0,k);
    //    gv(0);
    //}
    rep(10000){
        //cout<<"=======================================\n";
        //rep(10) cout<<arr[i]<<" "; cout<<endl;
        int type; cin>>type;
        if(type==0){
            int a; cin>>a;
            arr[a]--; delf(a);
        } else if(type==1) {
            int a; cin>>a;
            arr[a]++; addf(a);
        } else {
            int a; cin>>a;
            cout<<gi(a)<<endl;
        }
    }
}
