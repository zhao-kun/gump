#
# 压力测试的目标CORBA服务器名称 
#
test-case TM_SVR

#
# 压力测试的时长 (minutes)
#
time-elapse 5 

#
#  虚拟用户配置 
#  每类虚拟用户执行的服务名称，数量等信息
#
virtual-user
    tuxedo:1       #http + xml测试

#
# 每笔交易的思考时间(单位毫秒)
#
think-time  100


#
# 日志级别 0 - DEBUG; 1 - INFO; 2 - WARN; 3 - ERROR; 4 - ALWAYS
#
log-level 0

#
# redis 服务器的地址
#
#redis-server     127.0.0.1:6379
#redis-server     tcp://127.0.0.1:6379
redis-server     unix:///tmp/redis.sock 


#
# 报告生成目录可以是绝对目录或相对目录
#
report-dir  /home/ap/aps/log


#
# 运行模式(THREAD/PROCESS) 进程或线程模式
#
run-mode PROCESS


#
# PLUGIN 附加的库
#
plugin
    libluatuxedo.so luaopen_tuxedolib
