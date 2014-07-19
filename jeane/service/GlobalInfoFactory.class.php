<?php
    require_once '../config/SharedConfigurations.php';
    
    class GlobalInfoFactory {
        public static function get_global() {
            global $single_server;
            $redis = new Predis\Client($single_server);
    
            $testcase = $redis -> get('TC_NAME');
            $global_str = $testcase . '::GLOBAL_INFO';
            $global_info = $redis -> hgetall($global_str);
            $global = new GlobalInfo($testcase);
            $starttime = "";
            foreach ($global_info as $key => $value) {
                if ($key == "STARTTIME") {
                    $global -> set_starttime($value);
                } else if ("SERVICE_NUM" == $key) {
                    continue;
                } else {
                    $global -> add_service($key, $value);
                }
            }
            $global->set_redis($redis);
            return $global;        
        }
    };
?>