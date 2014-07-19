action = function ()
    local fd = gb.sockopen("192.168.220.129", 3311);

    gb.socksend(fd, __DATA, 1000);

    local retstr = gb.sockrecv(fd, 6, 1000);
    local len = tostring(retstr);

    len = len - 6;
    retstr = gb.sockrecv(fd, len, 1000);
    gb.sockclose(fd);

    __SVC_RESULT = true;
    return 0;
end
