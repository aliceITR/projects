package airblair.myexpenses;

import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatDialogFragment;

public class UpdateDialog extends AppCompatDialogFragment {
    private EditText name;
    private EditText category;
    private EditText date;
    private EditText amount;
    private EditText note;
    private TextView id;
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



    @Override
    public android.app.Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());

        LayoutInflater inflator = getActivity().getLayoutInflater();
        View view = inflator.inflate(R.layout.updatedialog, null);

        name = view.findViewById(R.id.nameEnter);
        category = view.findViewById(R.id.categoryEnter);
        date  = view.findViewById(R.id.dateEnter);
        amount  = view.findViewById(R.id.amountEnter);
        note  = view.findViewById(R.id.noteEnter);

        name.setText(MainActivity.Uname);
        category.setText(MainActivity.Ucat);
        date.setText(MainActivity.Udate);
        amount.setText(MainActivity.Uprice);
        note.setText(MainActivity.Unote);
        builder.setView(view)
                .setTitle("Update Expense")
                .setNegativeButton("cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

            }
        })
                .setPositiveButton("enter", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String typedname = name.getText().toString();
                        String typedCategory = category.getText().toString();
                        String typedDate = date.getText().toString();
                        String typedAmount = amount.getText().toString();
                        String typedNote = note.getText().toString();



                            Toast toast = Toast.makeText(getContext(),"Make sure Name, Category, Date, and Amount are filled out!",Toast.LENGTH_LONG);
                            if(typedname.isEmpty())
                            {

                                toast.show();
                            }
                            else if(typedAmount.isEmpty()){ toast.show();}
                            else if(typedCategory.isEmpty()){ toast.show();}
                            else if(typedDate.isEmpty()){ toast.show();}
                            else
                            {
                                double typedAamount = Double.parseDouble(amount.getText().toString());
                                listener.updateTexts(typedname, typedCategory, typedDate, typedAamount, typedNote,  MainActivity.UID);
                            }


                    }
                });
name = view.findViewById(R.id.nameEnter);
category = view.findViewById(R.id.categoryEnter);
date  = view.findViewById(R.id.dateEnter);
amount  = view.findViewById(R.id.amountEnter);
note  = view.findViewById(R.id.noteEnter);
id  = view.findViewById(R.id.IdEnter);

        return builder.create();
    }

    public interface DialogListener{
        void updateTexts(String typedname, String typedCategory, String typedDate, double typedAmount, String typedNote, String typedId);
    }

}
