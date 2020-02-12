package com.airblair.baturubotsu;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatDialogFragment;

public class SetupV2 extends AppCompatDialogFragment implements Button.OnClickListener{

    TextView errorMessageText;
    // Text boxes
    EditText serverAddress,armor,power,scanPower;
    // The error dialog
    Dialog errorMessage;
    // Store picked variables
    int armorValue,powerValue,scanPowerValue;
    @Override
    public android.app.Dialog onCreateDialog(Bundle savedInstanceState) {

        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflator = getActivity().getLayoutInflater();
        final View view = inflator.inflate(R.layout.setup, null);

        serverAddress =  view.findViewById(R.id.serverAddress);
        armor =   view.findViewById(R.id.armorPicker);
        armor.setText("1");
        power =   view.findViewById(R.id.powerPicker);
        power.setText("1");
        scanPower =   view.findViewById(R.id.scanPicker);
        scanPower.setText("1");
        builder.setView(view)
                .setTitle("Connect")
                .setNegativeButton("cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {}})
                .setPositiveButton("enter", new DialogInterface.OnClickListener() {

                    @Override

                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            armorValue = Integer.parseInt(armor.getText().toString()) - 1;
                            powerValue = Integer.parseInt(power.getText().toString()) - 1;
                            scanPowerValue = Integer.parseInt(scanPower.getText().toString()) - 1;
                            //If it succeeds, we see if they fit our constraints
                            if ((armorValue >= 0 && armorValue <= 4) && (powerValue >= 0 && powerValue <= 4)
                                    && (scanPowerValue >= 0 && scanPowerValue <= 4) && (armorValue+powerValue+scanPowerValue <= 5)) {
                                String address = serverAddress.getText().toString();
/////////////////////////////////////////////////////////////////////////////////////////////////////////
                                listener.applyTexts(address, armorValue, powerValue, scanPowerValue);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
                            } else {
                                // Tell the user to check the input it doesn't succeed
                                errorMessageText.setText("Please enter valid allocations!");
                                errorMessage.show();
                            }
                        }
                        //If parsing fails, we tell the user to check their input
                        catch (java.lang.NumberFormatException e) {
                            errorMessageText.setText("Invalid input! Please check all forms.");
                            errorMessage.show();
                        }
                    }

                });
        return builder.create();
    }
    @Override
    public void onClick(View v) {
    }


    /////////////////Listener////////////////////////////
    private DialogListener listener;
    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        try {
            listener = (DialogListener)context;
        } catch (ClassCastException e) {
            throw new ClassCastException(context.toString() +  "must implemtn dialog listener");
        }
    }

    public interface DialogListener{
        void applyTexts(String s1, int i1, int i2, int i3);
    }
}
