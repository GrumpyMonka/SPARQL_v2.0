var indexOfWorkingBlock = [];
var max_size_blocks = 0;

function workingBlocks() 
{
    for (var i = 0; i < indexOfWorkingBlock.length; i++) 
    {
        var input = [];
        for (var j = 0; j < blocks_list[indexOfWorkingBlock[i]].input_blocks.length; j++) 
        {
            input.push(blocks_list[blocks_list[indexOfWorkingBlock[i]].input_blocks[j]].output);
        }
        blocks_list[indexOfWorkingBlock[i]].output.push(blocks_list[indexOfWorkingBlock[i]].script(input, indexOfWorkingBlock[i]));
        if ( indexOfWorkingBlock[i] < max_size_blocks )
            api.setOutputForDiagramItem( indexOfWorkingBlock[i], blocks_list[indexOfWorkingBlock[i]].output );
        blocks_list[indexOfWorkingBlock[i]].flagOfWorking = true;
    }  
    indexOfWorkingBlock = [];
}

function addInputDependBlockForComposite( name, index )
{
    for( var i = 0; i < blocks_list.length; i++ )
    {
        if ( blocks_list[i].output[0] == name )
        {
            blocks_list[index].input_blocks.push( blocks_list[i].input_blocks );
        }
    }
}

function addOutputDependBlockForComposite( name, index )
{
    var index_output_block;
    for( var i = 0; i < blocks_list.length; i++ )
    {
        if ( blocks_list[i].output[0] == name )
        {
            index_output_block = i;
            break;
        }
    }

    for( var i = 0; i < blocks_list.length; i++ )
    {
        for( var j = 0; j < blocks_list[i].input_blocks.length; j++ )
        {
            if ( blocks_list[i].input_blocks[j] == index_output_block )
            {
                blocks_list[i].input_blocks[j] = index;
            }
        }
    }
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
        if (!blocks_list[blocks_list[index].input_blocks[j]].flagOfWorking)
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
                checkForReadyBlocks();
                if ( indexOfWorkingBlock.length == 0 )
                    break;
                workingBlocks();
            }
        }
    }
}
