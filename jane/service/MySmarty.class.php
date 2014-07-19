<?php
    require_once __DIR__.'/../lib/Smarty/Smarty.class.php';
    function ContextPath() {
        return dirname($_SERVER['PHP_SELF']);
    }
    class MySmart extends Smarty {
        public function __construct() {
            parent::__construct();
            $this->setTemplateDir('../view/template');
            $this->setCompileDir('../view/compile');
        }
    }
?>