#!/bin/bash
egrep " unsigned char " -r ./ | xargs sed "s/\ unsigned\ char\ /\ BYTE\ /g"
egrep " char " -r ./ | xargs sed 's/\ char\ /\ BYTE\ /g'
egrep " unsigned int " -r ./ | xargs sed "s/\ unsigned\ int\ /\ UINT32\ /g"
egrep " unsigned long " -r ./ | xargs sed "s/\ unsigned\ long\ /\ UINT32\ /g"
egrep " unsigned short " -r ./ | xargs sed "s/\ unsigned\ short\ /\ UINT16\ /g"
