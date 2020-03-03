#include <iostream>
#include <stdlib.h>
#include <time.h>
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
Slice *nsl(Slice *s){
    Slice *d = new Slice;
    d->size = s->size;
    d->data = new char[d->size];
    for(int i=0;i<s->size;i++){ 
        d->data[i] = s->data[i];
        // d->data[i+1] = s->data[i+1];
        // d->data[i+2] = s->data[i+2];
        // d->data[i+3] = s->data[i+3];
    }
    return d;
}
bool eq(Slice *a, Slice *b){
    if(a->size!=b->size) return false;
    rep(a->size){ if(a->data[i]!=b->data[i]) return false; } return true;
}
void gv(int index){ rep(arr[index]) cout<<hasht[index][i].key->data<<" "; cout<<endl; }
bool les(LL a, LL b){
    int n=a.key->size; if(n<(b.key->size)) n=b.key->size;
    rep(n){
        if((a.key->data[i])<(b.key->data[i])) return true;    
        else if((a.key->data[i])>(b.key->data[i])) return false;    
    } return a.key->size < b.key->size;
}
void ins(int ind, Slice *d, Slice *k){
    // cout<<"insert called\n";
    // cout<<arr[ind]<<" BEFORE "<<endl;
    arr[ind]++;
    // cout<<arr[ind]<<endl;
    hasht[ind] = (LL*) realloc(hasht[ind],arr[ind]*sizeof(LL));

    hasht[ind][arr[ind]-1].key = k; hasht[ind][arr[ind]-1].data = d;
    for(register int i=arr[ind]-2;i>=0;i--) if(les(hasht[ind][i+1],hasht[ind][i])){
        LL tmp = hasht[ind][i+1]; hasht[ind][i+1]=hasht[ind][i]; hasht[ind][i]=tmp;
    }
}
bool rdel(int ind,Slice *k,bool tp=false){
    if(tp) cout<<k->data<<" "<<(int)k->size<<endl;
    if(k->data[0]=='H'&&k->size==1){ cout<<"CHECK FOR H rdel\n"; }
    // if(k->data[0]=='H'){ cout<<"CHECK FOR H rdel\n"; }
    bool found=false;
    rep(arr[ind]){
        if(eq(hasht[ind][i].key,k)) found=true;
        if(found) hasht[ind][i]=hasht[ind][i+1];
    }
    if(k->data[0]=='H'&&k->size==1) cout<<found<<endl;
    if(found) arr[ind]--,hasht[ind] = (LL*) realloc(hasht[ind],arr[ind]*sizeof(LL));
    return found;
}
Slice *rget(int ind,Slice *k){
    rep(arr[ind]){ if(eq(hasht[ind][i].key,k)) return hasht[ind][i].data; } return NULL;
}

// end link list

// class
class kvStore {
    public:
    	kvStore(uint64_t max_entries){};
        bool put(Slice &key,Slice &value){
            // cout<<"PUT\n";
            int index = 0;
            Slice *ckey = nsl(&key);
            Slice *cvalue = nsl(&value);
            bool tp=false;
            // if(key.data[0]=='H'&&key.size==1){ 
                // cout<<"CHECK FOR H put\n";
                // tp=true;
            // }
            if(rget(index,&key)==NULL){
                // if(key.data[0]=='H'&&key.size==1) cout<<"HERE\n";
                ins(index,cvalue,ckey);
                //addf(index);
                return false;
            } else {
                //cout<<"here\n";
                rdel(index,&key);
                ins(index,cvalue,ckey);
                return true;
            }
        };
        bool get(Slice &key , Slice & value){
            // cout<<"GET"<<'\n';
            int index = 0;
            // cout<<key.data<<endl;
            Slice *g = rget(index,&key);
            // if(g==NULL) cout<<"NOT FOUND\n";
            if(g==NULL) return false;
            value = *g;
            return true;
            // else if(eq(g,&value)) return true;
            // else return false;
        };
        bool del(Slice &key){
            int index = 0;
            return rdel(index,&key);
        };
        bool get(int n ,Slice& key, Slice& value ){
            key = *hasht[0][n].key;
            value = *hasht[0][n].data;
            return true;
        };
        bool del(int n){
            Slice *key = hasht[0][n].key;
            rdel(0,key);
            return true;
        };
};

// end class

// kvStore kv(10);
// #include<bits/stdc++.h>
// int main(){
//     int n=10000000;
//     double tdiff = 0;
//     struct timespec st, en;
//     rep(n){
//         Slice *d = new Slice, *k = new Slice; 
//         k->data = new char[32]; d->data = new char[128];
//         d->size = 128; k->size=32;
//         cin>>k->data>>d->data;
//         //cout<<k->data<<" "<<d->data<<endl;
//         //ins(0,*d,*k);
//         //if(i==999999) cout<<k->data<<endl;
//         clock_gettime(CLOCK_MONOTONIC, &st);
//         kv.put(*k,*d);
//         clock_gettime(CLOCK_MONOTONIC, &en);
//         tdiff += (en.tv_sec - st.tv_sec) + 1e-9*(en.tv_nsec - st.tv_nsec);
//         if(i%100000==0) cout<<"AT "<<i/100000<<endl;
//         //gv(0);
//         //cout<<d.data<<" "<<k.data<<endl;
//     }
//     cout<<tdiff<<endl;
// }
//     map<int,int> ast;
//     rep(76527504) ast[arr[i]]++;
//     for(auto i:ast) cout<<i.first<<" : "<<i.second<<endl;
//     //while(false){
//     //    Slice *d = new Slice, *k = new Slice; 
//     //    d->data = new char[255]; k->data = new char[64];
//     //    d->size = 255; k->size=64;
//     //    cin>>k->data>>d->data;
//     //    int typ; cin>>typ;
//     //    if(typ==0){
//     //        cout<<kv.get(*k,*d)<<endl;
//     //    } else if(typ==1) {
//     //        cout<<kv.del(*k)<<endl;
//     //    } else if(typ==2) {
//     //        int nv; cin>>nv;
//     //        cout<<kv.get(nv,*k,*d)<<endl;
//     //    } else if(typ==3){
//     //        cout<<"Deleting\n";
//     //        int nv; cin>>nv; cout<<kv.del(n)<<endl;
//     //    }
//     //}
//     //while(true){
//     //    Slice *d = new Slice, *k = new Slice; 
//     //    d->data = new char[64]; k->data = new char[255];
//     //    d->size = 10; k->size=10;
//     //    cin>>k->data;
//     //    Slice *a = rget(0,k);
//     //    if(a!=NULL) cout<<a->data<<endl;
//     //    del(0,k);
//     //    gv(0);
//     //}
//     //rep(10000){
//     //    //cout<<"=======================================\n";
//     //    //rep(10) cout<<arr[i]<<" "; cout<<endl;
//     //    int type; cin>>type;
//     //    if(type==0){
//     //        int a; cin>>a;
//     //        arr[a]--; rdelf(a);
//     //    } else if(type==1) {
//     //        int a; cin>>a;
//     //        arr[a]++; addf(a);
//     //    } else {
//     //        int a; cin>>a;
//     //        cout<<gi(a)<<"="<<gib(a)<<endl;
//     //    }
//     //}
// }