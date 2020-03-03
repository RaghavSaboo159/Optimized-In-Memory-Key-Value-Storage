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
string sliceToStra(Slice& a) {
    string ret = "";

    for (int i = 0; i < a.size; i++)
        ret += a.data[i];

    return ret;
}

void gv(int index){ rep(arr[index]) cout<<sliceToStra(*hasht[index][i].key)<<" "; cout<<endl; }
bool les(LL a, LL b){
    int n=a.key->size; if(n>(b.key->size)) n=b.key->size;
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
    for(int i=arr[ind]-2;i>=0;i--) if(les(hasht[ind][i+1],hasht[ind][i])){
        LL tmp = hasht[ind][i+1]; hasht[ind][i+1]=hasht[ind][i]; hasht[ind][i]=tmp;
    }
}
bool rdel(int ind,Slice *k,bool tp=false){
    if(tp) cout<<k->data<<" "<<(int)k->size<<endl;
    // if(k->data[0]=='H'&&k->size==1){ cout<<"CHECK FOR H rdel\n"; }
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

// hash
int dec(char a){    
    if(a<'a') return a-'A'+1; else return a-'a'+27;    
}    
int shash(Slice *s){    
    int ret = 0; int up=1; // [9, 486, 26244, 1417176, 76527504]        
    //cout<<s->data<<" "<<dec(s->data[0])<<endl;        
    if(s->size>4) ret+=(dec(s->data[4])/6);        
    if(s->size>3) ret+=(dec(s->data[3])*9);        
    if(s->size>2) ret+=(dec(s->data[2])*486);        
    if(s->size>1) ret+=(dec(s->data[1])*26244);        
    if(s->size>0) ret+=(dec(s->data[0])*1417176);        
    return ret;        
}
// end hash

// fenwick
void addf(int index){ index++;
    while(index<=76527504) fen[index]++,index+=(index&-index);
}
void delf(int index){ index++;
    while(index<=76527504) fen[index]--,index+=(index&-index);
}
int query(int index){ index++; int su=0;
    while(index>0) su+=fen[index],index-=(index&-index);
    return su;
}
int gib(int index){
    int l=0,r=76527504,ans=-1; int mid=(l+r)/2;
    while(l<=r){
         if(query(mid)>=index){ ans=mid;r=mid-1; }
         else l=mid+1;
         mid=(l+r)/2;
    }
    if(ans>=0) {
        if(ans==0 || (ans!=0 && query(ans-1)!=index)) return ans;
        else if (ans !=0 && query(ans-1)==index){
            l=0,r=ans-1;mid=(l+r)/2;
            while(l<=r){
                if(query(mid)==index){ ans=mid;r=mid-1; }
                else if(query(mid)>index) r=mid-1;
                else l=mid+1;
                mid=(l+r)/2;
            } return ans;
        }
    }
    else return -1;
}
// end fenwick

// class
class kvStore {
    public:
    	kvStore(uint64_t max_entries){};
        bool put(Slice &key,Slice &value){
            // cout<<"PUT\n";
            int index = shash(&key);
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
                addf(index);
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
            int index = shash(&key);
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
            int index = shash(&key);
            if(rdel(index,&key)){
                delf(index);
                return true;
            } else return false;
        };
        bool get(int n ,Slice& key, Slice& value ){
            n++;
            int index = gib(n);
            // cout<<index<<" "<<n<<" "<<query(index-1)<<endl;
            if(index>0) n-=(query(index-1)+1);
            else n--;
            key = *hasht[index][n].key;
            value = *hasht[index][n].data;
            // cout<<key.data<<endl;
            return true;
        };
        bool del(int n){
            n++;
            int index = gib(n);
            // cout<<index<<" "<<n<<" "<<query(index-1)<<endl;
            if(index>0) n-=(query(index-1)+1);
            else n--;
            Slice *key = hasht[index][n].key;
            // cout<<key->data<<endl;
            // int index=0;
            // Slice *key = hasht[index][n].key;
            rdel(index,key);
            delf(index);
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