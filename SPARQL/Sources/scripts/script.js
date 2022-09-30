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

function Block(func, output, input_blocks) {
    this.output = output; //Выходное значение блока
    this.input_blocks = input_blocks; //Индексы входных блоков
    this.flagOfWorking = false; //Отработал ли блок или нет
    this.script = func;
}

var blocks_list = [];
var indexOfEnd = 0;
