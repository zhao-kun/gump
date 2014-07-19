action = function ()
    res = http.request("http://192.168.220.1:8080/java_web_test/WebTester", __DATA, 30, "POST");
    print(res);
    __SVC_RESULT = true;
    return 0;
end
