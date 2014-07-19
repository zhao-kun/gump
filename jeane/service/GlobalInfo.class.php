<?php
   require_once "../lib/Predis/Client.php";
class GlobalInfo {
    private $services;
    private $tcname;
    private $starttime;
    private $reids;


    public function __construct($tcname) {
        $this->services = array();
        $this -> tcname = $tcname;
    }

    public function add_service($svc, $vusers) {
        array_push($this->services, array("serivce" => $svc, "vuser" => $vusers));
    }

    public function set_starttime($time) {
        $this -> starttime = $time;
    }

    public function get_services() {
        return $this -> services;
    }

    public function get_tcname() {
        return $this -> tcname;
    }

    public function get_start_time() {
        return $this -> starttime;
    }

    public function set_redis($redis) {
        $this->redis = $redis;
    }
    
    public function get_redis() {
        return $this->redis;
    }

};
?>