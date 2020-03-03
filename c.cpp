#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
#include<functional>
using namespace std;
#define ll long long int
#define ld long double
using namespace __gnu_pbds;
typedef tree<ll,null_type,less<ll>,rb_tree_tag,
tree_order_statistics_node_update> in_set;
#define rep(n) for(ll i=0;i<(n);i++)
#define jrep(n) for(ll j=0;j<(n);j++)
#define all(a) (a).begin(), (a).end()
#define dpp(a) cout<<(a).first<<" "<<(a).second<<endl;
#define append push_back
#define mod 1000000007
#define ff first
#define ss second
#define ffast ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define fast ios::sync_with_stdio(0);
ll pw(ll x,ll y,ll p=mod){ll r=1; x=x%p;while(y>0){ if(y&1)r=(r*x)%p;
y=y>>1;x=(x*x)%p;} return r; } vector<ll> uniq(vector<ll> arr)
{vector<ll>brr;sort(all(arr));brr.append(arr[0]);
for(auto i:arr)if(brr.back()!=i)brr.append(i);return brr;}

ll arr[1000006];
struct Slice{ uint8_t size; char* data; };

int dec(char a){
    if(a<'a') return a-'A'+1; else return a-'a'+27;
}
int shash(Slice *s){
    int ret = 0; int up=1; // [9, 486, 26244, 1417176, 76527504]    
    //cout<<s->data<<" "<<s->size<<endl;    
    if(s->size>4) ret+=(dec(s->data[4])/6);    
    if(s->size>3) ret+=(dec(s->data[3])*9);    
    if(s->size>2) ret+=(dec(s->data[2])*486);    
    if(s->size>1) ret+=(dec(s->data[1])*26244);    
    if(s->size>0) ret+=(dec(s->data[0])*1417176);    
    return ret;    
}

void solve(){
    Slice *d = new Slice, *k = new Slice;
    int sz; cin>>sz;
    k->data = new char[sz];
    k->size = sz;
    cin>>k->data;
    cout<<k->data<<" => ";
    cout<<shash(k)<<endl;
}
 
int main(){
    //ffast
    ll t=1;
    cin>>t;
    rep(t) solve();
}
 
 
// *p.find_by_order(n) : nth value (0 indexed)
// p.order_of_key(7): index of 7 (0 indexed)
