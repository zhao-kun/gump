<?php
require_once '../config/SharedConfigurations.php';
require_once 'GlobalInfo.class.php';
require_once 'GlobalInfoFactory.class.php';

class FetchData {
    private $global = null;

    public function __construct($global) {
        $this -> global = $global;
    }

    public function fetch_data() {
        $redis = $this->global->get_redis();
        $ns = $this -> global -> get_tcname();


        foreach ($this->global->get_services() as $key1 => $value1) {
            foreach ($value1 as $key => $value) {
                if ($key == 'serivce') {
                    $time_table_name = $ns . "::" . $value . "_TOTAL_TIME";
                    $cnt_table_name = $ns . "::" . $value . "_TOTAL_CNT";
                    $total_time_res = $redis -> hgetall($time_table_name);
                    $total_cnt_res = $redis -> hgetall($cnt_table_name);
                    ksort($total_time_res);
                    /*
                    if (count($total_time_res) == 0) {
                        contiue;
                    }
                    $all_keys = array_keys($total_time_res);

                    $max_time = (int)(end($all_keys));
                                       
                    $tps = array_fill(0, $max_time, 0);
                    $art = array_fill(0, $max_time, 0);
                     */
                    $tps = array();
                    $art = array();
                    foreach ($total_time_res as $key2 => $value2) {
                        if (isset($total_cnt_res[$key2])) {
                            /*  tps 应该统一处理60 */
                            $tps[] = round(((double)($total_cnt_res[$key2]) / 60), 2);
                            if ($total_cnt_res[$key2] == 0) {
                                $art[] = 0;
                            }else {
                                $art[] = round(((double)$value2 / (double)($total_cnt_res[$key2])) , 2);
                            }
                        } else {
                            $tps[] = 0;
                            $art[] = 0.00;
                        }
                    }
                    /*弹走最后一条数据，不准确*/
                    array_pop($tps);
                    $last_art = array_pop($art);
                    $vuser = $value1['vuser'];
                    /*如果$last_art为 0 ， 那么不能再做 0除了，就使用最后的tps吧*/
                    if ($last_art == 0) {
                        $last_tps = end($tps);
                    }else {
                        $last_tps = round(($vuser * 1000)/$last_art,2);
                    }
                    $tps[] = $last_tps;
                    $art[] = $last_art;
                    $service[] = array('name'=> $value, 'tps'=> $tps, 'art'=>$art);
                }
            }
        }

        $cmd = $ns . "::TOTAL";
        $total = $redis -> get($cmd);

        $cmd = $ns . "::TOTAL_FAIL";
        $total_fail = $redis -> get($cmd);

        $cmd = $ns . "::TOTAL_SUCC";
        $total_succ = $redis -> get($cmd);

        $result = array('total' => $total, 'total_fail' => $total_fail, 'total_succ' => $total_succ, 'series' => $service);
        return $result;
    }


    public function generate_csv() {
        $services = $this->global->get_services();
        $ns = $this->global->get_tcname();
        $start_time = $this->global->get_starttime();
        
        $filename = ROOT.ns.$start_time."csv";
        
        $fp = fopen($filename, "w+");
        if ($fp == null) {
            die("open ".$filename." error");
            return false;
        }
        
        foreach ($services as $key => $value) {
            if ($key == 'serivce') {
                $time_table_name = $ns . "::" . $value . "_TOTAL_TIME";
                $cnt_table_name = $ns . "::" . $value . "_TOTAL_CNT";
                $total_time_res = $redis -> hgetall($time_table_name);
                ksort($total_time_res);
                $total_cnt_res = $redis -> hgetall($cnt_table_name);
                fputcsv($fp, array("service[".$value."] -- begin --"));
                $linevalue = array();
                
                foreach ($total_time_res as $key2 => $value2) {
                    $linevalue[] = key2;
                    if (isset($total_cnt_res[$key2])) {
                        /*  tps 应该统一除60 */
                        $linevalue[] = round(((double)($total_cnt_res[$key2]) / 60), 2);
                        $linevalue[] = round(((double)$value2 / (double)($total_cnt_res[$key2])) , 2);
                    } else {
                        $linevalue[] = 0;
                        $linevalue[] = 0.00;
                    }
                    fputcsv($fp, $linevalue);
                }
                fputcsv($fp, array("service[".$value."] -- end --"));
            }            
        } 
        fclose($fp);
        return $filename;
    }


}

/*
$global = GlobalInfoFactory::get_global();
$fetch = new FetchData($global);
$result = $fetch -> fetch_data();
echo json_encode($result);
*/
?>