r=package.loadlib("/home/ap/aps/gump/lib/libluabase.so", "luaopen_baselib");      
r();
f=loadfile("init.lua");         
f();
init();
c=package.loadlib("/home/ap/aps/gump/lib/libluacorba.so", "luaopen_corbalib");      
c();
z=loadfile("action.lua");
z();
action();

