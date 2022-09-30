function XMLHttpRequest(http){  
    this.network = http;
}

XMLHttpRequest.prototype.setUrl = function(url){
    this.network.setUrl(url);
}

XMLHttpRequest.prototype.open = function(type, path){
    this.network.open(type, path);
}

XMLHttpRequest.prototype.setRequestHeader = function(name, value){
    this.network.setRequestHeader(name, value);
}

XMLHttpRequest.prototype.send = function(data){
    this.network.send(data);
    return this.network.waitAnswer();
}