### My GRBL1.1 settings
Some good settings(size,speed,orientation) for the Grbl1.1<br>
 
<img src="JimmyCraft0902.gif" width="240"/>  <img src="DIY3018GrblSettings.png" width="240"> <br>

### References
  - [Free Laser Engraving](http://lasergrbl.com/en/) <br>
  - [Engraver Master](http://www.bachinmaker.com/wiki/doku.php?id=engraver_master)<br>

Grbl 1.1h ['$' for help]
client> $$
$0=10 (Step pulse time, microseconds)
$1=25 (Step idle delay, milliseconds)
$2=0 (Step pulse invert, mask)
$3=2 (Step direction invert, mask)
$4=0 (Invert step enable pin, boolean)
$5=0 (Invert limit pins, boolean)
$6=0 (Invert probe pin, boolean)
$10=3 (Status report options, mask)
$11=0.010 (Junction deviation, millimeters)
$12=0.002 (Arc tolerance, millimeters)
$13=0 (Report in inches, boolean)
$20=0 (Soft limits enable, boolean)
$21=0 (Hard limits enable, boolean)
$22=0 (Homing cycle enable, boolean)
$23=0 (Homing direction invert, mask)
$24=25.000 (Homing locate feed rate, mm/min)
$25=500.000 (Homing search seek rate, mm/min)
$26=250 (Homing switch debounce delay, milliseconds)
$27=1.000 (Homing switch pull-off distance, millimeters)
$30=1000 (Maximum spindle speed, RPM)
$31=20 (Minimum spindle speed, RPM)
$32=1 (Laser-mode enable, boolean)
$100=800.000 (X-axis travel resolution, step/mm)
$101=800.000 (Y-axis travel resolution, step/mm)
$102=800.000 (Z-axis travel resolution, step/mm)
$110=2500.000 (X-axis maximum rate, mm/min)
$111=2500.000 (Y-axis maximum rate, mm/min)
$112=2000.000 (Z-axis maximum rate, mm/min)
$120=80.000 (X-axis acceleration, mm/sec^2)
$121=80.000 (Y-axis acceleration, mm/sec^2)
$122=100.000 (Z-axis acceleration, mm/sec^2)
$130=300.000 (X-axis maximum travel, millimeters)
$131=180.000 (Y-axis maximum travel, millimeters)
$132=44.000 (Z-axis maximum travel, millimeters)
