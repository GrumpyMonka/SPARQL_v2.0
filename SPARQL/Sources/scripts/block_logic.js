var indexOfWorkingBlock = [];

function workingBlocks() 
{
    for (var i = 0; i < indexOfWorkingBlock.length; i++) 
    {
        var input = [];
        for (var j = 0; j < blocks_list[indexOfWorkingBlock[i]].input_blocks.length; j++) 
        {
            input.push(blocks_list[blocks_list[indexOfWorkingBlock[i]].input_blocks[j]].output);
        }
        blocks_list[indexOfWorkingBlock[i]].output.push(blocks_list[indexOfWorkingBlock[i]].script(input));
        blocks_list[indexOfWorkingBlock[i]].flagOfWorking = true;
    }  
    indexOfWorkingBlock = [];
}

function checkForReadyBlocks() 
{  
    for (var i = 0; i < blocks_list.length; ++i) 
    {
        if (!blocks_list[i].flagOfWorking) 
        {
            if (checkInputBlocks(i)) 
            {
                indexOfWorkingBlock.push(i); 
            }
        }
    }
}

function checkInputBlocks(index) 
{
    for (var j = 0; j < blocks_list[index].input_blocks.length; ++j) 
    {
        if (!blocks_list[blocks_list[index].input_blocks[j]].output.length) 
        {
            return false;
        }
    }
    return true;
}

function main(typeOfWorking) 
{
    if (typeOfWorking === 'debug')
    {
        //Сделать сигнал по которому проверяется работа для дебага
    } else {
        if (typeOfWorking === 'run')
        {
            while(true) 
            {
                if (checkInputBlocks(indexOfEnd))
                {
                    break;
                }
                checkForReadyBlocks();
                workingBlocks();
            }
        }
    }
}
