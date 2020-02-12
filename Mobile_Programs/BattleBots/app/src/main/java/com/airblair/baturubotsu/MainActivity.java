package com.airblair.baturubotsu;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentManager;

import android.app.Activity;
import android.app.Dialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity  implements Button.OnClickListener, SetupV2.DialogListener {
    private boolean gameOver, readyToWrite;
    private String serverAddress, messageToSend;
    // Variables the user can add points to
    private int armor,power,scanPower,HP;
    private TextView currentHP;
    // Errors
    private Dialog errorMessage;
    private TextView errorMessageText;
    private Button errorMessageButton;
    //Move Buttons
    private Button moveUpLeft,moveUp,moveUpRight,moveLeft,moveRight,moveDownLeft,moveDown,moveDownRight;
    //connect buuton and scan
    private Button connect,scanButton;
    // shot bottuns
    private Button shotUpLeft,shotUp,shotUpRight,shotLeft,shotRight,shotDownLeft,shotDown,shotDownRight;
    // Network connection
    private Socket connection;
    // thread
    private Thread thread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.controller);
        //Error Stuff
        errorMessage = new Dialog(this);
        errorMessage.setContentView(R.layout.error);
        errorMessage.setTitle("Error!");
        errorMessageText =  errorMessage.findViewById(R.id.errorMessageText);
        errorMessageButton = errorMessage.findViewById(R.id.errorMessageButton);
        errorMessageButton.setOnClickListener(this);

        //Game stuff
        moveUpLeft = findViewById(R.id.moveUpLeft);
        moveUpLeft.setOnClickListener(this);
        currentHP = findViewById(R.id.currentHP);
        gameOver = false;
//////////////////////////////////////////////////////////////////////////////////////////////
        connect = findViewById(R.id.connecter);
        connect.setOnClickListener(this);
///////////////////////////////////////////////////////////////////////////////////////////////
        moveUp = findViewById(R.id.moveUp);
        moveUp.setOnClickListener(this);
        moveUpRight = findViewById(R.id.moveUpRight);
        moveUpRight.setOnClickListener(this);
        moveLeft = findViewById(R.id.moveLeft);
        moveLeft.setOnClickListener(this);
        moveRight = findViewById(R.id.moveRight);
        moveRight.setOnClickListener(this);
        moveDownLeft = findViewById(R.id.moveDownLeft);
        moveDownLeft.setOnClickListener(this);
        moveDown = findViewById(R.id.moveDown);
        moveDown.setOnClickListener(this);
        moveDownRight = findViewById(R.id.moveDownRight);
        moveDownRight.setOnClickListener(this);
        shotUpLeft = findViewById(R.id.shotUpLeft);
        shotUpLeft.setOnClickListener(this);
        shotUp =  findViewById(R.id.shotUp);
        shotUp.setOnClickListener(this);
        shotUpRight = findViewById(R.id.shotUpRight);
        shotUpRight.setOnClickListener(this);
        shotLeft = findViewById(R.id.shotLeft);
        shotLeft.setOnClickListener(this);
        shotRight =  findViewById(R.id.shotRight);
        shotRight.setOnClickListener(this);
        shotDownLeft =  findViewById(R.id.shotDownLeft);
        shotDownLeft.setOnClickListener(this);
        shotDown =  findViewById(R.id.shotDown);
        shotDown.setOnClickListener(this);
        shotDownRight =  findViewById(R.id.shotDownRight);
        shotDownRight.setOnClickListener(this);
//////////////////////////////////////////////////////
        // Scan button
        scanButton = findViewById(R.id.scanButton);
        scanButton.setOnClickListener(this);
        // Get ready to write
        readyToWrite = true;
        //first message
        messageToSend = "airblair" + " "  + armor + " " + power + " " + scanPower;
    }

    @Override
    public void onClick(View view) {
        // Error message dialog button
        if (view == errorMessageButton) {
            // If the okay button is pressed, end the game
            errorMessage.dismiss();
            gameOver = true;
            try {
                connection.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
            finish();
        }
/////////////////////////////////////////////////////
        //connect button
        else if (view == connect)
        {
            openConnect();
        }
/////////////////////////////////////////////////////

        // Move buttons
        else if (view == moveUpLeft) {
            messageToSend = "move -1 -1";
        } else if (view == moveUp) {
            messageToSend = "move 0 -1";
        } else if (view == moveUpRight) {
            messageToSend = "move 1 -1";
        } else if (view == moveLeft) {
            messageToSend = "move -1 0";
        } else if (view == moveRight) {
            messageToSend = "move 1 0";
        } else if (view == moveDownLeft) {
            messageToSend = "move -1 1";
        } else if (view == moveDown) {
            messageToSend = "move 0 1";
        } else if (view == moveDownRight) {
            messageToSend = "move 1 1";
        } else if (view == shotUpLeft) {
            messageToSend = "shot 315";
        } else if (view == shotUp) {
            messageToSend = "shot 0";
        } else if (view == shotUpRight) {
            messageToSend = "shot 45";
        } else if (view == shotLeft) {
            messageToSend = "shot 270";
        } else if (view == shotRight) {
            messageToSend = "shot 90";
        } else if (view == shotDownLeft) {
            messageToSend = "shot 225";
        } else if (view == shotDown) {
            messageToSend = "shot 180";
        } else if (view == shotDownRight) {
            messageToSend = "shot 135";
        }
////////////////////////////////////////////////////////
        // Scan button
        else if (view == scanButton) {
            messageToSend = "scan";
        }
    }
    @Override
    //If the back button is pushed, close the connection, end the game, and stop the thread
    protected void onStop() {
        super.onStop();
        gameOver = true;
        try {
            connection.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        thread = null;
    }

    private Handler handler = new Handler()
    {
        @Override
        public void handleMessage(Message msg) {
            int handlerMethod = msg.getData().getInt("handlerMethod");
            if (handlerMethod == 1) {
                errorMessageText.setText(msg.getData().getString("msg"));
                errorMessage.show();
            } else if (handlerMethod == 2) {
                Toast.makeText(getApplicationContext(), msg.getData().getString("msg"), Toast.LENGTH_SHORT).show();
            } else {
                System.out.println(msg.getData().getString("msg"));
            }
        }
    };

    public void makeMessage(String str, int handlerMethod) {
        Message msg = new Message();
        Bundle b = new Bundle();
        b.putString("msg", str);
        b.putInt("handlerMethod", handlerMethod);
        msg.setData(b);
        handler.sendMessage(msg);
    }




    class doNetwork  implements Runnable
    {
        public void run() {
            int port = 3012;
            makeMessage("host is "+ serverAddress +"\n", 4);
            try{
                InetAddress sA = InetAddress.getByName(serverAddress);
                makeMessage("Attempting to connect..." + serverAddress +"\n", 4);
                connection = new Socket(sA, port);
                //get the writer and reader
                PrintWriter out = new PrintWriter( new BufferedWriter( new OutputStreamWriter(connection.getOutputStream())),true);
                BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
                while (!gameOver) {
                    try {
                        String str = in.readLine();
                        String[] data = str.split(" ");
                        //If we get an info read, start parsing it
                        while (data[0].equals("Info")) {
                            //if we're dead or it's game over, end the game
                            makeMessage(str, 2);
                            if (data[1].equals("Dead") || data[1].equals("GameOver")) {
                                gameOver = true;
                                break;
                            }
                            str = in.readLine();
                            data = str.split(" ");
                        }
                        //If the info section ended the game, well, end the game
                        if (gameOver)
                        {
                            makeMessage("Game over!", 1);
                            break;
                        }
                        //If we get a status read, parse the data
                        if (data[0].equals("Status"))
                        {
                            readyToWrite = true;
                            if (Integer.parseInt(data[5]) < HP)
                            {
                                currentHP.setText(data[5]);
                                HP = Integer.parseInt(data[5]);
                            }
                        }
                        //After the read, write the data specified by they button we pushed
                        if (readyToWrite && !messageToSend.equals("noop"))
                        {
                            String[] messageData = messageToSend.split(" ");
                            //If we got a setup read, send our setup data
                            if (data[0].equals("setup"))
                            {
                                out.println(messageToSend);
                            }
                            //If we hit a move button, move our bot if it can
                            else if (messageData[0].equals("move"))
                            {
                                if (Integer.parseInt(data[3]) < 0)
                                {
                                    makeMessage("Can't move yet!", 2);
                                }
                                else
                                {
                                    out.println(messageToSend);
                                }
                            }
                            //If we hit a shoot button, shoot, if we can
                            else if (messageData[0].equals("shot"))
                            {
                                if (Integer.parseInt(data[4]) < 0)
                                {
                                    makeMessage("Can't shoot yet!", 2);
                                }
                                else
                                {
                                    out.println(messageToSend);
                                }
                            }
                            //Otherwise, if it's a scan command, run a scan and parse the data while
                            //printing it to the client
                            else if (messageData[0].equals("scan"))
                            {
                                out.println(messageToSend);
                                str = in.readLine();
                                data = str.split(" ");
                                while (data[0].equals("scan") && !data[1].equals("done"))
                                {
                                   makeMessage(str, 2);
                                    str = in.readLine();
                                    data = str.split(" ");
                                }
                                makeMessage(str, 2);
                            }
                            messageToSend = "noop";
                            readyToWrite = false;
                        }
                        out.println("noop");
                        Thread.sleep(400);
                    }
                    //If we can't read or write, end the game for the client to avoid issues
                    catch(Exception e)
                    {
                        makeMessage("Error happened sending/receiving\n", 4);
                        gameOver = true;
                    }
                }
            }
            catch (Exception e)
            {
                //If we can't connect, and the game isn't over, pop up a dialog
                //to let the client know and end the game for the client
                if (!gameOver)
                {
                    makeMessage("Unable to connect...", 1);
                    gameOver = true;
                }
            }
        }
    }
    public void openConnect()
    {
        SetupV2 dialog = new SetupV2();
        dialog.show(getSupportFragmentManager(),"Dialog" );
    }
    @Override
    public void applyTexts(String s1, int i1, int i2, int i3) {
        final String s11 = s1;
        final int i11 = i1;
        final int i12 = i2;
        final int i13 = i3;
        serverAddress = s11;
        armor = i11;
        power = i12;
        scanPower = i13;
        HP = armor+1;
        currentHP.setText(" "+HP);
        thread = new Thread ( new doNetwork());
        thread.start();
            }






}
