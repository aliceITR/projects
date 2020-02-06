# Final Project

Group name: OS2Death

group members : Sam Blair, Gage Brown
<br><br>Drivers 1-6 are working<br><br>
Attributes: The attributes that we chose for files are: Creator Name & Color <br>
Creator Name Seemed Reasonable <br>
Color because why not have a color on a file <br>
--DUE December 9, 2019--


---

**grading is out of 100 points** 
* create file (imples r/w in dm, pm r/w, getfree, and returnblock) (10 points):   yes
* openfile (and close 3points of) opentable, etc ignoring unlock/lock (10 points): yes
* readfile (10 points): yes
* writefile (10 points): yes
* seekfile (5 points): yes
* appendfile ( 5 points, since just call see to end and write): yes
* createdir (10 points) => implies create,open work as well: yes
* lock and unlock (5 each)  looped into open/close as well: yes
* rename (5 points): yes
* deletefile (5 points), remember lock again: yes
* deletedir (5 points), remember empty: yes
* attributes read/set (5 points each): yes
   * get attrib looks if there is nothing set.  so functionally a default.

other notes:
  * disk is
    * driver 1: disk is good, consistent, files are correct.  file inode is wrong.  name, f/d, number.  Should be name, number, f/d 
      * there do not appear to be default attributes for files.  hopfully the get attrib will.
    * driver 2: disk is consistent.  all parititions look correct.
    * driver 3: rename and delete appear to work.  sub directories has same issues a root.
    * driver 4: disk is consistent across all the partitions, directories appear to work
    * driver 5: partition 2 is fulled correctly.  no inconsistency were found.
    * driver 6: attributes are set and visiable on the disk.  
  * other
    * code looks
      * over all the code looks good.  pretty well commented and readable.  
    * why did you modify the drivers output?  diff...
  * group eval:  individual grades will be emailed.

