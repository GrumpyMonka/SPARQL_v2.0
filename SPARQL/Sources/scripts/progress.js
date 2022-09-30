function Bar(){
    this.max = 0;
    this.value = 0;
}

Bar.prototype.ping = function(){
    this.value++;
    progress.setValue(this.value);
}

Bar.prototype.setNewBar = function(data){
    this.max = data;
    this.value = 0;
    progress.setMaximum(this.max);
    progress.setValue(this.value);
}

var ProgressBar = new Bar();