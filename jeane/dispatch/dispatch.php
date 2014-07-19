<?php
require '../config/SharedConfigurations.php';
require_once '../service/MySmarty.class.php';
require_once '../service/GlobalInfo.class.php';
require_once '../service/GlobalInfoFactory.class.php';

define('ROOT',str_replace('\\','/',dirname(dirname(__FILE__))) . '/');
define('DEBUG',true);


if(defined('DEBUG')) {
    error_reporting(E_ALL);
} else {
    error_reporting(0);
}

session_start();
function display($global) 
{
    $smarty = new MySmart();

    if ($global == null) {
        $global = GlobalInfoFactory::get_global();
        $_SESSION['TEST_GLOBAL'] = $global;
    } else {
        $global = $_SESSION['TEST_GLOBAL'];
    }

    $smarty -> assign('tc_name', $global -> get_tcname());
    $smarty -> assign('service_num', count($global -> get_services()));
    $smarty -> assign('starttime', $global -> get_start_time());
    $smarty -> assign('svc', $global -> get_services());
    $smarty -> display('main.html');

}

/*每次刷新都应该重新去设置global信息，否则会读取到历史信息*/
display(null);
?>