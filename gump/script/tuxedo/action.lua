function action()
    local v = tuxedo.alloc("STRING", 50);
    tuxedo.setdata(v, __DATA);
    tuxedo.init();
    v = tuxedo.call("TOUPPER", v)
    if v == nil then
        print( "call service error" );
        __SVC_RESULT = false;
        tuxedo.term();
        return;
    end

    local s = tuxedo.getdata(v);
    print(s);
    __SVC_RESULT = true;
    tuxedo.free(v);
    tuxedo.term();
end

