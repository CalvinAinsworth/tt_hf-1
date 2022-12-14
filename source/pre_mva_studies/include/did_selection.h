// ttbar Powheg+Pythia8: 410472 - incl; 411076 - ttbb, 411077 - ttb, 411078 - ttc had 410470
if (process=="tt" && generator=="nominal" && (job_DID == "410472" || job_DID=="411076" || job_DID=="411077" || job_DID=="411078") ) correct_did = true;


// ttbar Powheg+Pythia8 (hdamp=3*mtop): 410482 (incl)
if (process=="tt" && generator=="3mtop" && job_DID=="410482") correct_did = true;


// ttbar Powheg+Herwig7.1.3: 411234 - inclusive, 411332 - ttbb, 411333 - ttb, 411334 - ttc
if (process=="tt" && generator=="phhw713" && (job_DID=="411234" || job_DID=="411332" || job_DID=="411333" || job_DID=="411334")) correct_did = true;


// ttbar Powheg+Herwig7.0.4: 410558, 411085, 411086, 411087
if (process=="tt" && generator=="phhw704" && (job_DID=="410558" || job_DID=="411085" || job_DID=="411086" || job_DID=="411087")) correct_did = true;


// ttbar aMC@NLO+Pythia8: 410465, 412069, 412070, 412071
if (process=="tt" && generator=="aMC@nloPy8" && (job_DID=="410465" || job_DID=="412069" || job_DID=="412070" || job_DID=="412071")) correct_did = true;


// ttbar: sherpa 2.2.10
if (process=="tt" && generator=="sherpa2210" && (job_DID=="700122" || job_DID=="700123" || job_DID=="700124" || job_DID=="700167")) correct_did = true;


// ttV
if (process=="ttV" && (job_DID=="410155" || job_DID=="410156" || job_DID=="410157" || job_DID=="410218" || job_DID=="410219" || job_DID=="410220" || job_DID=="410276" || job_DID=="410277" || job_DID=="410278") ) correct_did = true;


// ttH
if (process=="ttH" && (job_DID=="346345") ) correct_did = true;


// Diboson
if (process=="diboson" && (job_DID=="364250" || job_DID=="364253" || job_DID=="364254" || (std::stoi(job_DID)>=364283 && std::stoi(job_DID)<=364290 && job_DID!="364286") || job_DID=="345705" || job_DID=="345706" || job_DID=="345723" || job_DID=="363356" || job_DID=="363358") ) correct_did = true;


// Z+jets
if (process=="z_jets" && (std::stoi(job_DID)>=364100 && std::stoi(job_DID)<=364141) ) correct_did = true;


// Other 
if (process=="other" && (job_DID=="410560" || job_DID=="410408" || job_DID=="346678" || job_DID=="346676" || job_DID=="412043") ) correct_did = true;


//singletop

if (process=="singletop" && (job_DID == "410644" || job_DID == "410645" || job_DID == "410658" || job_DID == "410659")) correct_did = true;
