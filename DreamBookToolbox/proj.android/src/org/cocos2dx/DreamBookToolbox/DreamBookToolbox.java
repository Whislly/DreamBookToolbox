/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.DreamBookToolbox;

import java.util.ArrayList;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.speech.RecognizerIntent;
import android.widget.Toast;

public class DreamBookToolbox extends Cocos2dxActivity{

	private static Handler handler; 
    private final static int HANDLER_Active_Voice = 1;
    private static final int VOICE_RECOGNITION_REQUEST_CODE = 1234;  
    
    public static void startupVoiceService() { 
        Message msg = new Message(); 
        msg.what = HANDLER_Active_Voice; 
        handler.sendMessage(msg); 
    } 
    
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		handler = new Handler() { 
            public void handleMessage(Message msg) { 
                switch (msg.what) { 
                case HANDLER_Active_Voice: 
                	try{
                        Intent intent=new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);  
                        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
                        intent.putExtra(RecognizerIntent.EXTRA_PROMPT, "开始语音");  
                        startActivityForResult(intent, VOICE_RECOGNITION_REQUEST_CODE);  
                        }catch (Exception e) {  
                            e.printStackTrace();  
                            Toast.makeText(getApplicationContext(), "找不到语音设备", 1).show();  
                        }  
                    break; 
                } 
            } 
        };
	}
	
	@Override  
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {  
        // TODO Auto-generated method stub  
        //回调获取从谷歌得到的数据   
        if(requestCode==VOICE_RECOGNITION_REQUEST_CODE && resultCode==RESULT_OK){  
            //取得语音的字符  
            ArrayList<String> results=data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);  
              
            String resultString=""; 
            if(results.size() > 0){
            	resultString = results.get(0).toLowerCase();
            }
            //for(int i=0;i<results.size();i++){  
            //    resultString+=results.get(i);  
            //}  
            //Toast.makeText(this, resultString, 1).show();  
            voiceRecogition(resultString);
        }  
        super.onActivityResult(requestCode, resultCode, data);  
    }  
	
    static {
         System.loadLibrary("game");
    }
}
