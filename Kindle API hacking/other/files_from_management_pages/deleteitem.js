Fion.deleteItem = function( contentName ) {
    var asin = contentName.substring(11); // asin extracted from the argument
    var itemData = itemCache.getDataByASIN(asin); // the item in the library list
    // create vars to be sent in the ajax request to fiona-delete
    var lendingId = '';
    var lendingType = '';
    var lendingStatus = '';
    var loanId = '';
    // assign the above vars where appropriate
    if(itemData.loanId) 
    {
        loanId = itemData.loanId;
    }

    if(itemData.category === "kindle_pdoc"){
        mykJS.log("pdocs","delete");
    }

    var deleteSpanId = "deleteItemResp_" + asin; // id of the delete span in the actionMsgRow
    var deleteSpan = document.getElementById(deleteSpanId); // the delete span
    var sid = mykJS.sessionId;
    deleteSpan.innerHTML = mykJS.kindle_myk_popover_deleting_41283; // add message to the span
    deleteSpan.style.display = "block"; // display the message
    this.showActionMsgRow(deleteSpanId);

    var callback = function( resp )
    {
        var object;
        try
        {
            object = eval( "(" + resp + ")" );
        }
        catch( e )
        {
            return;
        }

        if ( object.error ) // display the error in the action row
      {
            deleteSpan.innerHTML = "<span style=" + amznJQ.chars.DQUOTE + "color: #f00;" + amznJQ.chars.DQUOTE + ">" + object.error + "</span>";
            var actionMsgRowId = deleteSpan.parentNode.parentNode.getAttribute('id');
            window.setTimeout( function() { 
                    UtilitiesDom.hide( deleteSpanId );
                    UtilitiesDom.hide( actionMsgRowId );}, 1500);
        }
        else
        {
            deleteSpan.innerHTML = "<span style=" + amznJQ.chars.DQUOTE + "color: green;"+ amznJQ.chars.DQUOTE + ">" + mykJS.kindle_myk_popover_delete_success_18475 + "</span>"; 
            window.setTimeout( function() { window.location.reload(); }, mykJS.fadeOut );
        }
    }
    // string containing the key value pairs of args for the ajax request
    var ajaxMsgString = "contentName=" + asin + "&loanId=" + loanId + "&sid=" + sid + "&isAjax=1" + "&category=" + itemData.category;
    Fion.gufsAjaxObj.makeRequest( ajaxMsgString, callback );
}
