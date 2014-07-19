action = function ()
    local v = cb.cb_call("CORBA_SVR", __DATA);
    local str = cb.cb_get_string(v, "_hostcode", "");
    cb.cb_free(v);

    cb.cb_set_string(__PKG, "C601", "", "ÍõÎå");
    local v2 = cb.cb_call2("CORBA_SVR", __PKG);
    local contents = cb.cb_pkg2str(__PKG);


    print("result:"..contents);
    __SVC_RESULT = true;
    return 0;
end
