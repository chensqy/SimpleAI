#include <iostream>
#include <string>
#include "engine.h"
#include "util.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef _WIN32
static const char* h = R"(<!doctype html><html lang="zh-CN"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>SimpleAI</title><style>
:root{--bg:#0a0d13;--panel:#0f1421;--muted:#98a2b3;--pri:#6ea2ff;--pri2:#9bc2ff;--you:#1a2f5a;--me:#171923;--line:#242838}
*{box-sizing:border-box}html,body{height:100%}body{margin:0;background:radial-gradient(1200px 600px at -10% -10%,#12203a,transparent),radial-gradient(1000px 500px at 110% 10%,#1a2a54,transparent),var(--bg);color:#e9eef7;
font-family:-apple-system,BlinkMacSystemFont,"SF Pro Text","Segoe UI","Inter","HarmonyOS Sans SC","PingFang SC","Microsoft YaHei","Noto Sans SC","Source Han Sans SC","MiSans","OPPOSans","Helvetica Neue",Arial,sans-serif}
.c{max-width:960px;margin:0 auto;height:100%;display:flex;flex-direction:column;padding:16px;gap:14px}
.h{display:flex;align-items:center;gap:10px}
.l{width:12px;height:12px;border-radius:50%;background:linear-gradient(135deg,var(--pri),var(--pri2));box-shadow:0 0 12px rgba(110,162,255,.6)}
.t{font-size:18px;font-weight:600;letter-spacing:.2px}
.p{flex:1;min-height:0;border:1px solid var(--line);border-radius:14px;padding:14px;display:flex;flex-direction:column;gap:10px;overflow:auto;background:linear-gradient(180deg,rgba(255,255,255,.03),rgba(255,255,255,.01))}
.r{display:flex;gap:10px;align-items:flex-end}
.r.u{justify-content:flex-end}
.a{width:28px;height:28px;border-radius:50%;display:flex;align-items:center;justify-content:center;font-size:12px;color:#fff;user-select:none}
.a.v{background:linear-gradient(135deg,#2b5fff,#66a1ff);box-shadow:0 6px 18px rgba(102,161,255,.25)}
.a.u{background:linear-gradient(135deg,#444,#777)}
.m{max-width:72%;padding:12px 14px;border-radius:14px;line-height:1.5;word-wrap:break-word;white-space:pre-wrap;border:1px solid var(--line);box-shadow:0 8px 24px rgba(0,0,0,.25)}
.m.v{background:linear-gradient(180deg,#1a1e2b,#121521)}
.m.u{background:linear-gradient(180deg,#1d2c4f,#15243f)}
.s{display:flex;gap:10px}
.i{flex:1;padding:12px 14px;border-radius:12px;border:1px solid var(--line);background:#0b0f18;color:#e9eef7;outline:none;resize:vertical;min-height:44px;max-height:220px}
.b{padding:12px 16px;border-radius:12px;border:1px solid #2b55c7;background:linear-gradient(135deg,var(--pri),#2f6fff);color:#fff;cursor:pointer}
.b:disabled{opacity:.6;cursor:not-allowed}
.k{display:inline-flex;align-items:center;gap:8px}
.dot{width:6px;height:6px;background:#9bb7ff;border-radius:50%;display:inline-block;animation:a 1.4s infinite ease-in-out}
.dot:nth-child(2){animation-delay:.2s}.dot:nth-child(3){animation-delay:.4s}
@keyframes a{0%,80%,100%{opacity:.2;transform:translateY(0)}40%{opacity:1;transform:translateY(-3px)}}
</style></head><body><div class="c"><div class="h"><div class="l"></div><div class="t">SimpleAI</div><div style="flex:1"></div><div style="color:var(--muted);font-size:12px">UTF-8</div></div><div id="p" class="p"></div><div class="s"><textarea id="i" class="i" rows="2" placeholder="说点什么..."></textarea><button id="b" class="b">发送</button></div></div><script>
const a=document.getElementById('p');const b=document.getElementById('i');const c=document.getElementById('b');
function y(e){return e.replace(/[\\"]/g,m=>m==='\\\\'?'\\\\\\\\':'\\\\\\"').replace(/\\n/g,'\\n').replace(/\\r/g,'\\r')}
function d(e,f){const g=document.createElement('div');g.className='r '+(f?'u':'v');const h=document.createElement('div');h.className='a '+(f?'u':'v');h.textContent=f?'你':'A';const i=document.createElement('div');i.className='m '+(f?'u':'v');i.textContent=e;if(f){g.appendChild(i);g.appendChild(h)}else{g.appendChild(h);g.appendChild(i)}a.appendChild(g);a.scrollTop=a.scrollHeight}
function e(){const g=document.createElement('div');g.className='r v';const h=document.createElement('div');h.className='a v';h.textContent='A';const i=document.createElement('div');i.className='m v k';i.innerHTML='<span class="dot"></span><span class="dot"></span><span class="dot"></span>';g.appendChild(h);g.appendChild(i);a.appendChild(g);a.scrollTop=a.scrollHeight;return g}
async function f(){const g=b.value.trim();if(!g)return;d(g,true);b.value='';c.disabled=true;const h=e();try{const i=await fetch('/api/chat',{method:'POST',headers:{'Content-Type':'text/plain; charset=utf-8'},body:g});if(!i.ok)throw new Error('net');const j=await i.json();h.remove();d(j.a,false)}catch(err){h.remove();d('网络错误',false)}finally{c.disabled=false;b.focus()}}
c.addEventListener('click',f);b.addEventListener('keydown',t=>{if(t.key==='Enter'&&!t.shiftKey){t.preventDefault();f()}});d('你好，我在。随便聊聊，像和朋友一样。',false);
</script></body></html>)";
static std::string i(std::string a){std::string b;for(char c: a){if(c=='\\')b+="\\\\";else if(c=='\"')b+="\\\"";else if(c=='\n')b+="\\n";else if(c=='\r')b+="\\r";else b+=c;}return b;}
static void j(SOCKET a,const std::string& b,const std::string& c,int d){std::string e="HTTP/1.1 "+std::to_string(d)+" OK\r\nContent-Type: "+c+"; charset=utf-8\r\nCache-Control: no-store\r\nContent-Length: "+std::to_string((int)b.size())+"\r\nConnection: close\r\n\r\n";std::string f=e+b;send(a,f.c_str(),(int)f.size(),0);}
static std::string k(sockaddr* a){char b[128];void* c=nullptr;if(a->sa_family==AF_INET){c=&((sockaddr_in*)a)->sin_addr;}else if(a->sa_family==AF_INET6){c=&((sockaddr_in6*)a)->sin6_addr;}if(c&&inet_ntop(a->sa_family,c,b,(socklen_t)sizeof(b)))return b;return "0.0.0.0";}
int main(){
    SetConsoleOutputCP(CP_UTF8);SetConsoleCP(CP_UTF8);
    std::ios::sync_with_stdio(false);std::cin.tie(nullptr);
    WSADATA a1; if(WSAStartup(MAKEWORD(2,2),&a1)!=0) return 1;
    SOCKET b1=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); if(b1==INVALID_SOCKET){WSACleanup();return 1;}
    sockaddr_in c1; c1.sin_family=AF_INET; c1.sin_port=htons(8080); c1.sin_addr.s_addr=htonl(INADDR_ANY);
    int d1=1; setsockopt(b1,SOL_SOCKET,SO_REUSEADDR,(const char*)&d1,sizeof(d1));
    if(bind(b1,(sockaddr*)&c1,sizeof(c1))==SOCKET_ERROR){closesocket(b1);WSACleanup();return 1;}
    if(listen(b1,32)==SOCKET_ERROR){closesocket(b1);WSACleanup();return 1;}
    std::cout<<"已启动: http://127.0.0.1:8080\n";std::cout.flush();
    A e1;
    for(;;){
        sockaddr_storage f1; int g1=sizeof(f1);
        SOCKET h1=accept(b1,(sockaddr*)&f1,&g1); if(h1==INVALID_SOCKET) break;
        std::string i1; char p[8192]; int q; size_t r=std::string::npos;
        for(;;){q=recv(h1,p,sizeof(p),0); if(q<=0) break; i1.append(p,p+q); r=i1.find("\r\n\r\n"); if(r!=std::string::npos) break;}
        if(r==std::string::npos){closesocket(h1);continue;}
        std::string s=i1.substr(0,r+4); std::string t=i1.substr(r+4);
        std::string u,v; {size_t w=s.find(' '); if(w!=std::string::npos){u=s.substr(0,w); size_t x=s.find(' ',w+1); if(x!=std::string::npos)v=s.substr(w+1,x-w-1);} }
        int y=0; {std::string z=s; for(auto& aa:z) aa=(char)std::tolower((unsigned char)aa); size_t ab=z.find("content-length:"); if(ab!=std::string::npos){size_t ac=s.find(":",ab); if(ac!=std::string::npos){size_t ad=ac+1; while(ad<s.size() && (s[ad]==' '||s[ad]=='\t')) ad++; size_t ae=ad; while(ae<s.size() && isdigit((unsigned char)s[ae])) ae++; y=std::stoi(s.substr(ad,ae-ad));}}}
        while((int)t.size()<y){q=recv(h1,p,sizeof(p),0); if(q<=0) break; t.append(p,p+q);}
        std::string ip=k((sockaddr*)&f1);
        if(u=="GET" && (v=="/"||v=="/index.html")){u::q(ip+" GET /"); j(h1,h,"text/html",200);closesocket(h1);continue;}
        if(u=="POST" && v=="/api/chat"){
            std::string aa=t;
            u::q(ip+" IN "+aa);
            std::string ab=e1.b(aa);
            u::q(ip+" OUT "+ab);
            std::string ac="{\"a\":\""+i(ab)+"\"}";
            j(h1,ac,"application/json",200);
            closesocket(h1);continue;
        }
        std::string ad="Not Found"; std::string ae="HTTP/1.1 404 Not Found\r\nContent-Type: text/plain; charset=utf-8\r\nContent-Length: "+std::to_string((int)ad.size())+"\r\nConnection: close\r\n\r\n"+ad; send(h1,ae.c_str(),(int)ae.size(),0); closesocket(h1);
    }
    closesocket(b1);WSACleanup();return 0;
}
#else
int main(){return 0;}
#endif
