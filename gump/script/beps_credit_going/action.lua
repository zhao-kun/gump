action = function ()
    v = cb.cb_call("CORBA_SVR", __DATA);
    str = cb.cb_get_string(v, "_hostcode", "");
    cb.cb_free(v);
    __SVC_RESULT = true;
    return 0;
end
