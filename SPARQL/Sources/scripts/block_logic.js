var indexOfWorkingBlock = [];
var max_size_block = blocks_list.length;

function workingBlocks() 
{
    log( "Blocks for working " + indexOfWorkingBlock );
    for (var i = 0; i < indexOfWorkingBlock.length; i++) 
    {
        log( "Run block - " + indexOfWorkingBlock[i] );
        var input = [];
        var vec = [];
        for (var j = 0; j < blocks_list[indexOfWorkingBlock[i]].input_blocks.length; j++) 
        {
            input.push(blocks_list[blocks_list[indexOfWorkingBlock[i]].input_blocks[j]].output);
            vec.push(blocks_list[indexOfWorkingBlock[i]].input_blocks[j] );
        }
        blocks_list[indexOfWorkingBlock[i]].output.push(blocks_list[indexOfWorkingBlock[i]].script(input, indexOfWorkingBlock[i], vec));
        if ( indexOfWorkingBlock[i] < max_size_blocks )
        {
            api.setOutputForDiagramItem( indexOfWorkingBlock[i], blocks_list[indexOfWorkingBlock[i]].output );
        }
        blocks_list[indexOfWorkingBlock[i]].flagOfWorking = true;
    }  
    indexOfWorkingBlock = [];
}

function addInputDependBlockForComposite( name, index, index_composite )
{
    log( "func_input " + name + " " + index  + " " + index_composite );
    var composite_input = blocks_list[index_composite].input_blocks;
    for( var i = 0; i < composite_input.length; i++ )
    {
        if ( blocks_list[composite_input[i]].output[0] == name )
        {
            if ( blocks_list[composite_input[i]].input_blocks.length )
            {
                blocks_list[index].input_blocks.push( blocks_list[composite_input[i]].input_blocks );
                log( "blocks[" + index + "] - " + blocks_list[index].input_blocks + " + " + index_composite );
            }
        }
    }
}

function addOutputDependBlockForComposite( name, index, index_composite )
{
    log( "func_output " + name + " " + index  + " " + index_composite );
    var index_output_block;
    for( var i = 0; i < blocks_list.length; i++ )
    {
        var flag = false;
        for ( var k = 0; k < blocks_list[i].input_blocks.length; k++ )
        {
            if ( blocks_list[i].input_blocks[k] == index_composite )
            {
                log( "blocks[" + i + "] - " + blocks_list[i].input_blocks[k] + " == " + index_composite );
                flag = true;
            }
        }
        if ( !flag )
        {
            continue;
        }

        if ( blocks_list[i].output[0] == name )
        {
            index_output_block = i;

            // дабы на стрелках нормально выводились надписи (out у composite блок)
            blocks_list[i].input_blocks = [ index ];
            blocks_list[i].output = [];
            blocks_list[i].script = function( x, indx )
            {
                return x;
            };
            blocks_list[i].flagOfWorking = false;

            log( "blocks_input " + blocks_list[i].input_blocks );
            break;
        }
    }

//    for( var i = 0; i < blocks_list.length; i++ )
//    {
//        for( var j = 0; j < blocks_list[i].input_blocks.length; j++ )
//        {
//            if ( blocks_list[i].input_blocks[j] == index_output_block )
//            {
//                blocks_list[i].input_blocks[j] = index;
//            }
//        }
//    }
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
        if ( typeof blocks_list[blocks_list[index].input_blocks[j]] !== 'object' )
        {
            log( "BLOCKS = " + blocks_list[index].input_blocks );
            log( "Check Blocks " + index + " - " + blocks_list[index].input_blocks[j] )
            log( "NOT OBJECT" );
            return false;
        }

        if (!blocks_list[blocks_list[index].input_blocks[j]].flagOfWorking)
        {
            return false;
        }
    }
    return true;
}

function main(typeOfWorking) 
{
    api.clearDeps();
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
