#include<iostream>
#include<bits/stdc++.h>

#include <stdlib.h>
using namespace std;
#define rep(n) for(int i=0;i<(n);i++)
#define jrep(n) for(int j=0;j<(n);j++)
int fen[76527504+6],arr[76527504+5];

//struct Slice{ int size; char* data; };
struct Slice{ uint8_t size; char* data; };
struct LL{ Slice *data; Slice *key; LL *link; };
LL *hasht[76527504+5];

bool lesser(Slice *a,Slice *b){
    int n=a->size; if(n<(b->size)) n=b->size;
    rep(n){
        if((a->data[i])<(b->data[i])) return true;
        else if((a->data[i])>(b->data[i])) return false;
    }
    return a->size < b->size;
}

bool eq(Slice *a, Slice *b){
    if(a->size!=b->size) return false;
    rep(a->size){
        if(a->data[i]!=b->data[i]) return false;
    } return true;
}
void ins(int index, Slice *d, Slice *k)
{
    // cout << d << " " << k << endl;
    if (hasht[index] == NULL)
    {
        hasht[index] = new LL;
        hasht[index]->data = d;
        hasht[index]->key = k;
        hasht[index]->link = NULL;
        cout << "1" << hasht[index]->key->data << endl;
    }
    else if (lesser(k, hasht[index]->key) == true)
    {
        LL *temp = new LL;
        temp->key = hasht[index]->key;
        temp->data = hasht[index]->data;
        temp->link = NULL;
        hasht[index]->key = k;
        hasht[index]->data = d;
        hasht[index]->link = temp;
        cout << "0" << hasht[index]->key->data << hasht[index]->link->key->data << endl;
    }
    else
    {
        LL *itr = hasht[index];

        while (1)
        {
            if (itr->link == NULL) // cout << temp->key->data << endl;

            {
                // cout << itr->key->data << endl;
                if (lesser(itr->key, k) == true)
                {
                    itr->link = new LL;
                    itr->link->data = d;
                    itr->link->key = k;
                    itr->link->link = NULL;
                    // cout << "2" << itr->key->data << " " << itr->link->key->data << endl;
                }
                else
                {
                    LL *temp = new LL;
                    temp->key = itr->key;
                    temp->data = itr->data;
                    temp->link = NULL;
                    // temp->link = NULL;
                    hasht[index]->data = d;
                    hasht[index]->key = k;
                    hasht[index]->link = temp;
                    // cout << "3" << hasht[index]->key->data << " " << hasht[index]->link->key->data << endl;
                }
                break;
            }
            LL *cmp = itr->link;
            // cout << "5" << cmp->key->data << " " << itr->key->data << endl;
            if (lesser(cmp->key, k) == false && lesser(itr->key, k) == true)
            {
                // cout << cmp->key->data << " " << itr->key->data << endl;
                LL *node = new LL;
                // itr->link = node;
                node->data = d;
                node->key = k;
                node->link = cmp;
                itr->link = node;
                // cout << "4" << itr->key->data << " " << itr->link->key->data << " " << itr->link->link->key->data << endl;
                break;
            }
            LL *temp = new LL;
            temp = cmp;
            cmp = cmp->link;
            itr = temp;
            // while(!(itr->link==NULL)) itr=itr->link;
        } // itr->link = new LL; itr->link->data=d; itr->link->key = k;
    }
}


void gv(int index){
    LL *head = hasht[index];
    long long cnt=0;
    // cout<<arr[index]<<endl:
    while(head!=NULL){
        cout<<(head->key->data)<<" ";
         head=head->link;
         cnt++;
    } cout<<cnt<<endl;
}

bool oget(int index,Slice *key ){
    LL *head = hasht[index];
    while(head!=NULL){
        //cout<<head->data->data<<":"<<(head->key->data)<<"  "; 
        if(eq(head->key,key)) return true;
        head=head->link;
    } return false;
}
bool oget1(int index,Slice *key ,Slice *value){
    LL *head = hasht[index];
    while(head!=NULL){
        //cout<<head->data->data<<":"<<(head->key->data)<<"  "; 
        if(eq(head->key,key) && eq(head->data,value)) return true;
        head=head->link;
    } return false;
}


void odel(int index,Slice *key){
    LL *head = hasht[index];
    if(head==NULL){ delete hasht[index]; hasht[index]=NULL; return; }
    if(eq(head->key,key)){ hasht[index]=head->link; delete head; return; }
    while(head->link!=NULL){
        if(eq(head->link->key,key)){
            LL *td = head->link;
            head->link = head->link->link; delete td;
            return;
        }
        head=head->link;
    }
}

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

char decc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

Slice *rkey(){
    Slice *ret = new Slice;
    ret->size = 64; ret->data = new char[64];
    rep(64){ ret->data[i] = decc[rand()%52]; }
    return ret;
}

Slice *rval(){
    Slice *ret = new Slice;
    ret->size = 255; ret->data = new char[255];
    rep(255){ ret->data[i] = decc[rand()%52]; }
    return ret;
}



void addf(int index){
    arr[index]++; index++;
    while(index<=76527504) fen[index]++,index+=(index&-index);
}

void delf(int index){
    arr[index]--; index++;
    while(index<=76527504) fen[index]--,index+=(index&-index);
}

int query(int index){ index++; int su=0;
    while(index>0)
        su+=fen[index],index-=(index&-index);
    return su;
}

int lb(int *n){
    int mx = 76527504; int mi=0; int md=(mx+mi)/2;
    while(mi<mx){
        int s = query(md);
        if(s<(*n)){
            mi=md+1;
        } else mx=md-1;
        md = (mx+mi)/2;
    }
    md+=3; while(query(md)>(*n)) md--;
    (*n)-=query(md);
    if(*n==0){ while(arr[md]==0) md--; }
    else{ while(arr[md]==0) md++; }
    // cout<<md<<" "<<arr[md]<<" "<<*n<<endl;
    return md;
}

Slice getNK(int n){
    int index = lb(&n);
    LL *head = hasht[index];
    //cout<<n<<endl;
    rep(n-1) head=head->link;
    return *(head->data);
}

bool getN(int n ,Slice *key , Slice * value){
    cout<<"n value"<<";"<<n<<"\n";
    int index = lb(&n);
    cout<<"happen"<<":"<< index<<"\n";
    LL *head = hasht[index];
    cout<<"happen1"<<":"<< index<<"\n";
    gv(index);

    cout<<n<<" "<<arr[index]<<endl;
    rep(n-1) head = head->link;
    cout<<head->key<<";"<<key<<"\n";
    cout<<head->data<<";"<<value<<"\n";

    if (eq((head->key), key) && eq((head->data),value))
    	return true;
    else
    	return false;
}

class kvStore {
    public:
    	kvStore(uint64_t max_entries){};
        bool put(Slice &key,Slice &value){
            cout<<"PUT"<<'\n';
            int index = shash(&key);
            bool ret = oget(index,&key);
            if(ret) odel(index,&key);
            ins(index, &value, &key);
            addf(index);
            return ret;
        };
        bool get(Slice &key , Slice & value){
            cout<<"GET"<<'\n';
            
            int index = shash(&key);
            return oget1(index, &key , &value);
        };
        bool del(Slice &key){
            cout<<"DELK"<<'\n';
            
            int index = shash(&key);
            delf(index);
            if(oget(index,&key)){
                odel(index,&key);    
                return true;
            }
            return false;
        };
        bool get(int n ,Slice& key, Slice& value ){
            cout<<"getN"<<'\n';
            
            return getN(n , &key , &value);
        };
        bool del(int n){
            cout<<"DELN"<<'\n';
            
            Slice key = getNK(n);
            int index = shash(&key);
            odel(index,&key);
            delf(index);
            return true;
        };
};
int main(){    
    rep(10){    
        Slice d,k;     
        d.data = new char[64]; k.data = new char[255];    
        cin>>d.data>>k.data;    
        ins(0,&d,&k);    
        //cout<<d.data<<" "<<k.data<<endl;    
    }    
    gv(0);   
} 


//kvStore ks;
//map<string,string> mp;
//#include<time.h>
//#include <unistd.h>
// int main(){
    //LL *hashta = (LL*)maintoc((76527504+5)*sizeof(long long int));
    //rep(1000000){
        //Slice *a = rkey(); Slice *d = rval();
        //vrr.push_back({*a,*d});
        //cout<<a->data<<" "<<d->data<<endl;
        //int index = shash(a);
        //ins(index,d,a);
        //string key = a->data;
        //string val = d->data;
        //mp[key]=val;
        //ks.put(*a,*d);
        //usleep(1000000);
        //if(i==1231) nth = *d;
        //cout<<ks.get(*a)<<endl;
        //odel(index,a);
        //addf(index);
        //cout<<get(shash(a),a)<<endl;
        //if(i%100000==0) cout<<i<<endl;
    //}
    //int iter=0;
    //for(auto i:mp){
    //    if(iter==1231){
    //        cout<<i.second<<endl;
    //    }
    //    iter++;
    //}
    //cout<<ks.get(1232).data<<endl;
    //cout<<ks.del(1231)<<endl;
    //cout<<ks.get(1231).data<<endl;
    //cout<<"DONE!!\n";
// }
