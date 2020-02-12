package airblair.myexpenses;

import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatDialogFragment;

public class deleteDialog extends AppCompatDialogFragment {
    private EditText deleteName;

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
        View view = inflator.inflate(R.layout.deletedialog, null);

        builder.setView(view)
                .setTitle("What Expense Do you want to delete")
                .setNegativeButton("cancel", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

            }
        })
                .setPositiveButton("enter", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String typedname = deleteName.getText().toString();



                            Toast toast = Toast.makeText(getContext(),"Make sure the ID is entered!",Toast.LENGTH_LONG);
                            if(typedname.isEmpty())
                            {
                                toast.show();
                            }
                            else
                            {
                                listener.deleteExpense(typedname);
                            }
                        

                    }
                });
deleteName = view.findViewById(R.id.deleteEnter);


        return builder.create();
    }

    public interface DialogListener{
        void deleteExpense(String typedname);
    }

}
