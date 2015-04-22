1. Create Go_Learning, Go_Original_DB, Go_New_DB under caffe

2. /caffe/Go_Learning/XXX/: 
	a. Go_training.prototxt : Net architecture
	b. Go_training_Solver.prototxt : Training Strategy

3. /caffe/tools: go_database_generator.cpp


compile steps:

step1. source imleo
step2. change directory to caffe
step3. make
step4. train the solver by: ./caffe train -solver ../../Go_Learning/Go_training_Solver.prototxt