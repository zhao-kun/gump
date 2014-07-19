#
# 测试案例的名称
#
test-case TM_SVR

#
# 压力测试的时长 (minutes)
#
time-elapse 10 

#
#  虚拟用户配置 
#  每类虚拟用户执行的服务名称，数量等信息
#
virtual-user
    beps_credit_going:2     #10个虚拟用户执行小额贷记汇出交易
#    hvps_credit_going:1     #10个虚拟用户执行小额贷记汇出交易

#
# 每笔交易的思考时间(单位毫秒), 注意thinktime的时间会降低系统的每秒交易处理量
#
#think-time  200


#
# 日志级别 0 - DEBUG; 1 - INFO; 2 - WARN; 3 - ERROR; 4 - ALWAYS
#
log-level 0

#
# redis 服务器的地址
#      tcp://xxx.xxx.xxx.xxx:xxxx - tcp 方式链接redis服务器，远程方式
#      unix:///filepath/          - unix 域方式进行连接，本地方式 
#
#redis-server     tcp://127.0.0.1:6379
redis-server     unix:///tmp/redis.sock 


#
# 报告生成目录可以是绝对目录或相对目录(废弃)
#
report-dir  /home/ap/aps/log


#
# 运行模式(THREAD/PROCESS) 
#    THREAD - 虚拟用户以进程方式运行
#    PROCESS - 虚拟用户以线程模式运行 
#
run-mode PROCESS


#
# PLUGIN 插件库的动态库名称，和入口函数，建议放在${RUNNERDIR}/lib目录下,
#        并且LD_LIBRARY_PATH中指定${RUNNERDIR}/lib为动态库搜索的路径
#
plugin
    libluabase.so  luaopen_baselib
    libluacorba.so luaopen_corbalib
