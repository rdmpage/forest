<?php

$filename = 'test-without-authors.gml';

$file_handle = fopen($filename, "r");

$status = 0;

$nodes = array();
$edges = array();

$text = '';

while (!feof($file_handle)) 
{
	$line = fgets($file_handle);
	
	switch ($status)
	{
		case 0:
			if (preg_match('/node\s+\[/', $line))
			{
				$status = 1; // in a node
				$text = $line;
			}
			if (preg_match('/edge\s+\[/', $line))
			{
				$status = 2; // in a edge
				$text = $line;
			}		
			break;
			
		case 1:
			$text .= $line;
			if (preg_match('/^\s*\]$/', $line))
			{				
				$nodes[] = $text;
				$text = '';
				$status = 0;
			}
			break;			
			
		case 2:
			$text .= $line;
			if (preg_match('/^\s*\]$/', $line))
			{				
				$edges[] = $text;
				$text = '';
				$status = 0;
			}
			break;
			
		default:
			break;
	}
}

echo "graph [\n";
echo "directed 1\n";
foreach ($nodes as $n)
{
	echo $n;
}
foreach ($edges as $e)
{
	echo $e;
}
echo "]\n";


?>			
