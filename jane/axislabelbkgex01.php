<?php // content="text/plain; charset=utf-8"
require_once ('jpgraph/jpgraph.php');
require_once ('jpgraph/jpgraph_line.php');

$ydata = array(12,19,3,9,15,10);

// The code to setup a very basic graph
$graph = new Graph(958,198);
$graph->SetScale('intlin');
$graph->SetMargin(30,15,40,30);
$graph->SetMarginColor('#59C2E6');
$graph->SetFrame(true,'white',3);


$graph->title->Set("交易");
$graph->title->SetFont(FF_SIMSUN,FS_BOLD,12);

$graph->subtitle->SetFont(FF_SIMSUN,FS_NORMAL,10);
$graph->subtitle->SetColor('white');
$graph->subtitle->Set('"LABELBKG_NONE"');

$graph->SetAxisLabelBackground(LABELBKG_NONE,'orange','red','lightblue','red');

// Use Ariel font
$graph->xaxis->SetFont(FF_SIMSUN,FS_NORMAL,9);
$graph->yaxis->SetFont(FF_SIMSUN,FS_NORMAL,9);
$graph->xgrid->Show();

// Create the plot line
$p1 = new LinePlot($ydata);
$graph->Add($p1);

// Output graph
$graph->Stroke();

?>
