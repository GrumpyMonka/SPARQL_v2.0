function log(data){
    output.insertPlainText(data + "\n");
}

function clear(){
	output.clear();
}

function TOutput(){
}

TOutput.prototype.clear = clear;
TOutput.prototype.log = log;

var console = new TOutput();

