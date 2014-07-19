<?php
class SqlHhelp {
    private $db_name;
    private $db_password;
    private $db_user;
    private $db_host;
    private $mysqli;

    public function __construct($db_host, $db_name, $db_password, $db_user) {
        $this -> db_name = $db_name;
        $this -> db_password = $db_password;
        $this -> db_user = $db_user;
        $this -> db_host = $db_host;
    }

    public function get_db_name() {
        return $this -> db_name;
    }

    public function get_db_password() {
        return $this -> db_passowrd;
    }

    public function db_open() {
        $this -> mysqli = new Mysqli($this -> db_host, $this -> db_user, $this -> db_password, $this -> db_name);
        if (!$this -> mysqli) {
            die($this -> mysqli -> errno);
        }
    }
    
    public function execute_dql($sql, $args) {
        $prepstmt = $this->mysqli->prepare($sql);
        if (is_array($args)) {
            for ($i=0; $i < count($args); $i++) { 
                if ($prepstmt->bind_param()){
                    
                }
            }
        }else {
            
        }
    }
    
    public function db_close() {
        $this->mysqli->close();
    }
};
?>