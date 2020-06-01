7  | <div style="width:150px">Application Layer </div>| `Length` | `To` | `From` | `Segment` | `Port` | `Public Key`| `Metadata` | <div style="width:180px"> `Encrypted Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | ------ | ------ | ------ | --------- | ---------- | -----

6  | <div style="width:150px">Presentation Layer </div>| `Length` | `To` | `From` | `Segment` | `Port` | `Public Key`|<div style="width:285px"> `Encrypted Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | ------ | ------ | --------- | ---------- | -----

5  | <div style="width:150px">Session Layer </div>| `Length` | `To` | `From` | `Segment` | `Port` | `Public Key`|<div style="width:285px"> `Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | ------ | --------- | ---------- | ----- | -----

4  | <div style="width:150px">Transport Layer </div>| `Length` | `To` | `From` | `Segment` |  `Port` | <div style="width:405px"> `Raw data`</div> | `CRC`
-- | --------------- | -------- | ---- | ------ | --------- | ---------- | ----- | -----

3  |  <div style="width:150px"> Network Layer </div>| `Length` | `To` | `From` | <div style="width:570px"> `Raw data`</div> | `CRC`
-- | ------------- | -------- | ---- | ------ | ---------- | -----

2  | <div style="width:150px"> Datalink Layer </div> | `Length` | <div style="width:695px"> `Raw data`</div> | `CRC`
-- | -------------- | -------- | ---------- | -----

1  | <div style="width:150px"> Physical Layer </div>| `Length`|<div style="width:755px"> `Raw data`</div>
-- | ------- |-------------- | ----------