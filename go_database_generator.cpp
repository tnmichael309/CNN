#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "caffe/caffe.hpp"
#include "caffe/util/io.hpp"
#include "caffe/blob.hpp"

#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/rng.hpp"

using namespace caffe;  // NOLINT(build/namespaces)
using std::pair;
using boost::scoped_ptr;
using namespace std;
using namespace cv;

int main(int argc, char** argv){
	
	string data_base_name = "./Go_New_DB/Go_data_base_";
	data_base_name = data_base_name + string(argv[1]);
	
	// Create new DB
	scoped_ptr<db::DB> db(db::GetDB("leveldb"));
	db->Open(data_base_name.c_str(), db::NEW);
	scoped_ptr<db::Transaction> txn(db->NewTransaction());
	
	unsigned long long int key = 1;
	Datum datum;
	fstream infile;
	infile.open(("./Go_Original_DB/original_go_db_"+string(argv[1])).c_str(), ios::in);

	while(true){
		char position[500], label[10];
		char delim1 = ' ';
		char delim2 = '\n';
		
		if(!infile.getline(position,500,delim1)) break;
		if(!infile.getline(label,10,delim2)) break;
		
		int counter = 0;
		uchar real_position[380];
		
		for(int i = 0; i < 380; i++){
			
			if(position[i] == '1') real_position[counter] = (uchar) 255;
			else real_position[counter] = (uchar) 0;
			counter ++;
		}
		
		Mat cv_img = Mat(19,20, CV_8UC1,real_position);
		cout << cv_img << endl;
		CVMatToDatum(cv_img, &datum);
		datum.set_label(atoi(label));
		
		// Put in db
		string out;
		CHECK(datum.SerializeToString(&out));
		char keyCharArray[100];
		sprintf(keyCharArray,"%llu",key);
		txn->Put(string(keyCharArray), out);
		
		if (key % 1000 == 0) {
			// Commit db
			txn->Commit();
			txn.reset(db->NewTransaction());
			LOG(ERROR) << "Processed " << key-1 << " files.";
		}
		key++;
	}
	
	// Commit db
	txn->Commit();
	txn.reset(db->NewTransaction());
	LOG(ERROR) << "Processed " << key-1 << " files.";
	
}
