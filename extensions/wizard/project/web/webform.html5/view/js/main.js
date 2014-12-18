require.config({
	paths: {
		jquery: 'jQuery/jquery.min' //可在这里自定义库文件的实际路径,省略'.js'文件后缀名 
	}
}); 
  
require( [ "jquery","aauto" ], function($,aauto){ 

	$().ready( function() {  
		
		getSysPath  = function() {
		
			//调用aauto控器器对象中的接口函数
			aauto.getSysPath({
				onEnd:function(path){
					$("section textarea").html( path );
				}
			});
			
			/*
			var canvas = document.createElement("canvas"); 
			document.body.appendChild(canvas);
			
			if(!canvas.getContext){
				window.G_vmlCanvasManager.initElement(canvas);
			} 
			var ctx = canvas.getContext("2d");  
			
			ctx.fillStyle = "#aa0000";
			ctx.beginPath();
			ctx.arc(100, 100, 25, 0, Math.PI*2, true);
			ctx.closePath();
			ctx.fill(); 
			*/
		}
	} )
} );