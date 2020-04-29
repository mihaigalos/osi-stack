Very rough model of an ISO/OSI communication stack.

1  | <div style="width:125px"> Phisical Layer </div>|<div style="width:500px"> `Raw data`</div>
-- | -------------- | ----------

2  | <div style="width:125px"> Datalink Layer </div> | `Length` | <div style="width:350px"> `Raw data`</div> | `CRC`
-- | -------------- | -------- | ---------- | -----

3  |  <div style="width:125px"> Network Layer </div>| `Length` | `To` | `From` | <div style="width:225px"> `Raw data`</div> | `CRC`
-- | ------------- | -------- | ---- | ------ | ---------- | -----

4  | <div style="width:125px">Transport Layer </div>| `Length` | `To` | `From` | `Segment` | <div style="width:130px"> `Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | --------- | ---------- | -----

<br/>

5  | <div style="width:150px">Session Layer </div>| <div style="width:495px"> ... </div> | 
-- | -------------------------------------------- | ---------------------------------- 

6  | <div style="width:150px">Presentation Layer </div>| <div style="width:495px"> ... </div> | 
-- | -------------------------------------------- | ---------------------------------------

7  | <div style="width:150px">Application Layer </div>| <div style="width:495px"> ... </div> | 
-- | -------------------------------------------- | -------------------------------------- 
