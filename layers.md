7  | <div style="width:150px">Application Layer </div>| `Length` | `To` | `From` | `Segment` | `Port` | `Public Key`| `Metadata` | <div style="width:140px"> `Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | ------ | ------ | ------ | --------- | ---------- | -----

6  | <div style="width:150px">Presentation Layer </div>| `Length` | `To` | `From` | `Segment` | `Port` | `Public Key`|<div style="width:245px"> `Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | ------ | ------ | --------- | ---------- | -----

5  | <div style="width:150px">Session Layer </div>| `Length` | `To` | `From` | `Segment` | `Port` |<div style="width:365px"> `Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | ------ | --------- | ---------- | -----

4  | <div style="width:150px">Transport Layer </div>| `Length` | `To` | `From` | `Segment` | <div style="width:435px"> `Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | --------- | ---------- | -----

3  |  <div style="width:150px"> Network Layer </div>| `Length` | `To` | `From` | <div style="width:530px"> `Raw data`</div> | `CRC`
-- | ------------- | -------- | ---- | ------ | ---------- | -----

2  | <div style="width:150px"> Datalink Layer </div> | `Length` | <div style="width:655px"> `Raw data`</div> | `CRC`
-- | -------------- | -------- | ---------- | -----

1  | <div style="width:150px"> Physical Layer </div>| `Length`|<div style="width:718px"> `Raw data`</div>
-- | ------- |-------------- | ----------