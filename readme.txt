------------------------------------------------------
Theodoros Anagnostopoulos / sdi1400009  / project2
------------------------------------------------------

(sorry gia ta greeklish)


---------------- HOW TO RUN ----------------------
{{compilation}}:
$make


{{run}}:
$./cluster -h   (show help)


$./cluster -i xxx -d xxx -c xxx  -a

!!!!!!!  -complete  =    -a    !!!!!!! 



------------------ INPUT FILES ---------------------------

global metavlhtes sto global .h/.cpp
DImension = 203

Config file :

	den einai ipoxrewtiko
	
	EXTRA epiloges:
		max kmeans iterations  (n)
		check g()'s in lsh??  (0/1)


----------------- SXEDIASH KWDIKA 1/2 --------------------
	------- PARADOXES  - WARNINGS  ----------

	break critiria()::
exw valei thn  mikrh sinolikh
allagh twn kentrwn, thn mikrh allagh twn dianismatwn se 
clusters, kai to max iterations ( default =  -->globals.cpp)

exw prospa8hsei na apofygw ta infinte loops tou lsh
kanontas ena check me tis previous times twn number_of_changes


	radius lsh assign search
kanw hardcode to R otan paei poli konta sto max_distance between
datapoins. px otan R= 1  den exei nohma na paei sto R=2
giati 8a epistrafoun polla datapoints gia ka8e kentro. kai apo
tis ektipwseis pou ekana ( y_cluster_changes_per_iteration)
, ta datapoints poy phgainoun se diaforetiko cluster gia R=2 search
den einai poly perissotera apo auta gia R=1.
opote save some time. 


	hash_euclidean_cube()  
koitaei mexri kai Hamming distance = 2 





----------------- SXEDIASH KWDIKA 2/2 --------------------

silhouete +  medoids:  unordered_map gia na krataw tis apostaseis

elegxw ta G(query) by default, alla pisteuw oti den prepei na to kanoume
auto gia to Radius search.  Opote evala +epilogh sto c_file

to lsh/cube search se mia sinarthsh h opoia kaleitai ws 
	lsh_search( query , radius , dataset , mode=1 or 2  )
auto den to vlepoun oi alles sinarthseis ka8ws h ilopoihsh
tous (project 1) egine mesa se vivlio8hkh .a  /  .so

diafores apostaseis sto distance_fucntions.cpp

parsing twn arxeiwn ths main, sto main's function.
	
o kmeans exei mia genikh sinarthsh run()
kai diafores parallages gia init() , update(), assign() ::

kmeans.cpp::
	<<<genikes sinarthseis , silhouette , contructors>>> 
kmeans_functions.cpp::
	<<< main project  >>>  


xrhsimopoiountai arketes global metavlhtes. 
giati oxi?  




--------------- PARAMETERS / METRHSEIS ------------------

>sto results2.txt

kala w: ~1.5 - 2.0  
kalo range ri:  sini8ws mikra..

~
ta w prepei na einai se range tetoio wste ena megalo 
pososto twn kontinoteron geitonwn enos query na 
vriskontai sto distance = [3w , 4w]



gia k=20:: mesos oros S = 0.06



~

ta klasika runs, px loyd - medoid
den kanoun pote la8h, ka8ws einai brute force search,
opote to Si antiproswpeuei ka8ara sthn epilogh tou K
kai isws ligo apo thn arxkh init twn kentrwn
genikotera fainontai pio sta8era apo to LSH.
Otan sigrinoume K1-means loyd run me K2-means loyd run
sthn ousia apofasizoume posa cluster einai kalitero na exoume


~
te medoids xalane to lsh / cube search. infinite loop enalla3 ta kentra
den proteinetai medoids + lsh or cube

ta medoids siglinoun para poli grhgora se epanalhpseis , 
alla kanoun poli perissotero xrono, opote den 3erw an worth it se sxesh
me to loyd. isws an eixame noise sto dataset, auta na eixane kalitero S
pros stigmhn einai poli xeirotera (isws exw kanei la8os?)
 
~
to capacity_percent (px. n/8  ,  n/4) den metraei sxedon ka8olou sto Si
to LSH anamenomena htan arketa grhgoro, alla diolou spania emfanizontai
fails:: 
	1 fail = 1 dianisma den kathgoriopoih8hke ston nearest neighbor.


opote exei auto na aphreazei arketa ta pososta Si

~
sto range assign : mporei na einai arketa grhgoro alla tautoxrona kai
ipervolika argo, ka8ws peutei se infinite loops kai to termatizw me
si8hkes sigklishs.


~
hypercube den ta paei poli kala.
th mia kanei S = 0.03  kai time = 76" kai thn allh
kanei S = 0.06 kai time = 17"
den poli asxolh8hka me auto. mporei na ftaei to 
	probes = (elegxe ola ta hamming dist= 2)


~
sto lsh_assign radius search::
otan to R  paei poli konta sto max_distance between
datapoins. (px otan R= 1 ) den exei nohma na paei sto R=2 giati:

 8a epistrafoun polla datapoints gia ka8e kentro. 
 apo tis ektipwseis pou ekana ( y_cluster_changes_per_iteration)
, ta datapoints poy phgainoun se diaforetiko cluster gia R=2 search
den einai poly perissotera apo auta gia R=1 .






