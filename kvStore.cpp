#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
using namespace std;
#define rep(n) for(register int i=0;i<(n);i++)
#define jrep(n) for( register int j=0;j<(n);j++)


// globals

int fen[8503056+5],arr[8503056+5];
char decc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
struct Slice{ uint8_t size; char* data; };
struct LL{ Slice *data; Slice *key; };
LL *hasht[8503056+5];
bool mmutex = false;
int nop;
// end globals

// Linked List
Slice *nsl(Slice *s){
    Slice *d = new Slice;
    d->size = s->size;
    d->data = new char[d->size];
    register int i=0;
    for(;i+3<s->size;i+=4){ 
        d->data[i] = s->data[i];
        d->data[i+1] = s->data[i+1];
        d->data[i+2] = s->data[i+2];
        d->data[i+3] = s->data[i+3];
    }
    for(;i<s->size;i++) d->data[i]=s->data[i];
    return d;
}
bool eq(Slice *a, Slice *b){
    if(a==NULL||b==NULL) return false;
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
    arr[ind]++;
    if(arr[ind]>1){
        hasht[ind] = (LL*) realloc(hasht[ind],arr[ind]*sizeof(LL));
    } else hasht[ind] = (LL*) malloc(sizeof(LL));
    // if(arr[ind]==1){
        // hasht[ind] = (LL*) malloc(sizeof(LL)*3);
    // } //else hasht[ind] = (LL*) malloc(sizeof(LL));
    hasht[ind][arr[ind]-1].key = k; hasht[ind][arr[ind]-1].data = d;
    for(int i=arr[ind]-2;i>=0;i--) if(les(hasht[ind][i+1],hasht[ind][i])){
        LL tmp = hasht[ind][i+1]; hasht[ind][i+1]=hasht[ind][i]; hasht[ind][i]=tmp;
    }
}
bool rdel(int ind,Slice *k,bool tp=false){
    if(tp) cout<<k->data<<" "<<(int)k->size<<endl;
    bool found=false;
    if(k==NULL) return false;
    rep(arr[ind]){
        if(ind>=0&&ind<=8503056&&eq(hasht[ind][i].key,k)){
        	found=true;
        	// delete hasht[ind][i].key;
        	// delete hasht[ind][i].data;
        }
        if(found) hasht[ind][i]=hasht[ind][i+1];
    }
    if(found) arr[ind]--;//,hasht[ind] = (LL*) realloc(hasht[ind],arr[ind]*sizeof(LL));
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
    // if(s->size>5) ret+=(dec(s->data[4])/6);        
    // if(s->size>4) ret+=(dec(s->data[3])*9);        
    // if(s->size>3) ret+=(dec(s->data[2])*486);        
    // if(s->size>2) ret+=(dec(s->data[1])*26244);        
    // if(s->size>1) ret+=(dec(s->data[0])*1417176);        
    // if(s->size>5) ret+=(dec(s->data[4])/6);        
    if(s->size>4) ret+=(dec(s->data[3]));        
    if(s->size>3) ret+=(dec(s->data[2])*54);        
    if(s->size>2) ret+=(dec(s->data[1])*2916);        
    if(s->size>1) ret+=(dec(s->data[0])*157464);        
    return ret;
}
// end hash

// fenwick
void addf(int index){ index++;
    while(index<=8503056) fen[index]++,index+=(index&-index);
}
void delf(int index){ index++;
    while(index<=8503056) fen[index]--,index+=(index&-index);
}
int query(int index){ index++; int su=0;
    while(index>0) su+=fen[index],index-=(index&-index);
    return su;
}
int gib(int index){
    int l=0,r=8503056,ans=-1; int mid=(l+r)/2;
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
        // pthread_mutex_t lock[55];
    public:
    	kvStore(uint64_t max_entries){
            // rep(55) pthread_mutex_init(&lock[i], NULL) ;
        };
        bool put(Slice &key,Slice &value){
            // return true;
            // if(mmutex){ cout<<"CAUGHT PUT\n"; return false; } mmutex=true;
            //cout<<"PUT\n";
            // if(mmutex) cout<<"ERROR!!!\n";
            Slice *ckey = nsl(&key);
            Slice *cvalue = nsl(&value);
            // Slice *ckey = &key;
            // Slice *cvalue = &value;
            //if(ckey->data[ckey->size-1]=='\n'){ 
                //cout<<"ENDS WITH nl\n"; 
            //ckey->size--; 
            nop++;
            //}
            int index = shash(ckey);
            // pthread_mutex_lock(&lock[0]);
            //if(key.size==3 && key.data[0]=='B' && key.data[1]=='e') cout<<value.data<<endl;
            //cout<<"PUT into ||" <<index<<"||\n";
            //cout<<"KEY = ||"<<ckey->data<<"||\n";
        
            // Slice *ckey = &key;
            // Slice *cvalue = &value;
            // bool tp=false;
            // if(key.data[0]=='H'&&key.size==1){ 
                // cout<<"CHECK FOR H put\n";
                // tp=true;
            nop++;
            // }
            if(rget(index,ckey)==NULL){
                //if(key.size==3 && key.data[0]=='B' && key.data[1]=='e') cout<<"NEW VALue\n";
                // if(key.data[0]=='H'&&key.size==1) cout<<"HERE\n";
                ins(index,cvalue,ckey);
                addf(index);
                // pthread_mutex_unlock(&lock[0]); 
                // mmutex=false;
                return false;
            } else {
                //if(key.size==3 && key.data[0]=='B' && key.data[1]=='e') cout<<"EXISTING VALue\n";
                //cout<<"here\n";
                rdel(index,ckey);
                ins(index,cvalue,ckey);
                // mmutex=false;
                // pthread_mutex_unlock(&lock[0]); 
                return true;
            }
        };
        bool get(Slice &key , Slice & value){
            // return true;
            // if(mmutex){ cout<<"CAUGHT GET\n"; return false; } mmutex=true;
            //cout<<"GET\n";
            //if(mmutex) return false; mmutex=true;
            // if(mmutex) cout<<"ERROR!!!\n";
            //cout<<"GET"<<'\n';
            //if(key.size<10) return false;
            //key.size--;
            int index = shash(&key);
            // pthread_mutex_lock(&lock[0]);
            //cout<<key.data<<endl;
            Slice *g = rget(index,&key);
            //if(g==NULL) cout<<"NOT FOUND\n"; else cout<<"FOUND\n";
            //if(key.size==2 && key.data[0]=='B' && key.data[1]=='e') cout<<g->data<<endl;
            if(g==NULL){
                // pthread_mutex_unlock(&lock[0]); 
                return false; 
            }
            value = *g;
            // pthread_mutex_unlock(&lock[0]); 
            // mmutex=false;
            return true;
            // else if(eq(g,&value)) return true;
            // else return false;
        };
        bool del(Slice &key){
            // return true;
            // return true;
            // if(mmutex){ cout<<"CAUGHT DEL\n"; return false; } mmutex=true;
            //cout<<"DEL\n";

            //if(mmutex) return false; mmutex=true;
            // if(mmutex) cout<<"ERROR!!!\n";
            //key.size--;
            int index = shash(&key);
            // pthread_mutex_lock(&lock[0]);
            // try{
            if(rdel(index,&key)){
                delf(index);
                //key.size++;
                // pthread_mutex_unlock(&lock[0]);
                // mmutex=false;
                return true;
            } 
            // } catch(...){}
            // pthread_mutex_unlock(&lock[0]);
            //key.size++;
            // mmutex=false;
            return false;
        };
        bool get(int n ,Slice& key, Slice& value ){
            // return true;
            // if(mmutex){ cout<<"CAUGHT\n"; return false; } mmutex=true;
            // if(mmutex) return false; mmutex=true;
            // if(mmutex) cout<<"ERROR!!!\n";
            // if(n>=query(76527504)-1){ mmutex=false; return false; }
            //cout<<"GET N\n";
            n++;
            int index = gib(n);
            // pthread_mutex_lock(&lock[0]);
            //cout<<index<<" " <<n<<" "<<query(index-1)<<endl;
            if(index>0) n-=(query(index-1)+1);
            else n--;
            if(index<0){ 
                // pthread_mutex_unlock(&lock[0]); 
                return false; 
            }
            if(n>=0&&n<arr[index]){
                key = *hasht[index][n].key;
                value = *hasht[index][n].data;
            }// cout<<key.data<<endl;
            // mmutex=false;
            // pthread_mutex_unlock(&lock[0]); 
            return true;
        };
        bool del(int n){
            // return true;
            // if(mmutex){ cout<<"CAUGHT\n"; return false; } mmutex=true;
            // if(n>=query(76527504)-1){ mmutex=false; return false; }
            //cout<<"DEL N\n";

            //if(mmutex) return false; mmutex=true;
            // if(mmutex) cout<<"ERROR!!!\n";
            n++;
            int index = gib(n);
            // pthread_mutex_lock(&lock[0]);
            // cout<<index<<" "<<n<<" "<<query(index-1)<<endl;
            if(index>0) n-=(query(index-1)+1);
            else n--;
            // if(index<0){ pthread_mutex_unlock(&lock); return false; }
            // try{
            // cout<<"REACHED HERE\n";
            if(n>=0){
                // cout<<index<<" Deleting!!\n";
                if(index<0||index>=8503056) return false;
                // Slice *key = hasht[index][n].key;
                for(int i=n;i<arr[index]-1;i++) hasht[index][i]=hasht[index][i+1]; arr[index]--;
                // cout<<key->data<<endl;
                // int index=0;
                // Slice *key = hasht[index][n].key;
                // rdel(index,key);
                delf(index);
            }
        // } catch(...){}
            // mmutex=false;
            // pthread_mutex_unlock(&lock[0]);
            return true;
        };
};

// end class

//kvStore kv(10);
//#include<bits/stdc++.h>
//int main(){
//    int n=10000000;
//    double tdiff = 0;
//    struct timespec st, en;
//    rep(n){
//        Slice *d = new Slice, *k = new Slice; 
//        k->data = new char[32]; d->data = new char[128];
//        d->size = 128; k->size=32;
//        cin>>k->data>>d->data;
//        //cout<<k->data<<" "<<d->data<<endl;
//        //ins(0,*d,*k);
//        //if(i==999999) cout<<k->data<<endl;
//        clock_gettime(CLOCK_MONOTONIC, &st);
//        kv.put(*k,*d);
//        clock_gettime(CLOCK_MONOTONIC, &en);
//        tdiff += (en.tv_sec - st.tv_sec) + 1e-9*(en.tv_nsec - st.tv_nsec);
//        if(i%100000==0) cout<<"AT "<<i/100000<<endl;
//        //gv(0);
//        //cout<<d.data<<" "<<k.data<<endl;
//    }
//    cout<<tdiff<<endl;
//}
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