action = function ()
    gb.usleep(1000 * 200);
    if gb.exist("aa")  then
        __SVC_RESULT = true;
    else 
        __SVC_RESULT = false;
    end
    return 0;
end
