<?php
    require '../config/SharedConfigurations.php';
    require_once '../service/GlobalInfo.class.php';
    require_once '../service/GlobalInfoFactory.class.php';
    require_once '../service/FetchData.class.php';
    
    header("Content-type:application/json");
    
    if (isset($_SERVER['TEST_GLOBAL'])) {
        $global = $_SERVER['TEST_GLOBAL'];        
    }else {
        $global = GlobalInfoFactory::get_global();        
    }
    $fetch = new FetchData($global);
    $result = $fetch -> fetch_data();
    echo json_encode($result);
?>