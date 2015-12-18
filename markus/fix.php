<?php

$filename = 'test-without-authors.gml';

$file_handle = fopen($filename, "r");

$status = 0;

$nodes = array();
$edges = array();

$text = '';

$source = 0;
$target = 0;

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
				
				$source = 0; 
				$target = 0;
				$label = '';
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
			//$text .= $line;
			if (preg_match('/source\s+(?<source>\d+)/', $line, $m))
			{				
				$source = $m['source'];
			}
			if (preg_match('/target\s+(?<target>\d+)/', $line, $m))
			{				
				$target = $m['target'];
			}
			if (preg_match('/label\s+"(?<label>.*)"/', $line, $m))
			{				
				$label = $m['label'];
			}
						
			if (preg_match('/^\s*\]$/', $line))
			{		
			
				switch($label)
				{
					case 'parent_of':
						$edges[] = "edge [\n\tsource $source\n\ttarget $target\n\tlabel \"$label\"\n]\n";
						break;
						
					default:
						$edges[] = "edge [\n\tsource $target\n\ttarget $source\n\tlabel \"$label\"\n]\n";
						break;
				}					
			
				/*
				// skip badness
				if (preg_match('/parent_of/', $text))
				{
					$edges[] = $text;
				}
				*/
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
