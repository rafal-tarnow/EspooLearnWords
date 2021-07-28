package com.example.espoolearnwordsandroid;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class NewPolishWordActivity extends AppCompatActivity
{
    EditText polishWord;
    EditText translationWord;
    EditText pronunciation;

    Button saveBtn;
    Intent intent;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_new_polish_word);

        polishWord = (EditText)findViewById(R.id.txtPolishWord);
        translationWord = (EditText)findViewById(R.id.txtTranslation);
        pronunciation = (EditText)findViewById(R.id.txtPronunciation);

        saveBtn = (Button)findViewById(R.id.btnSave);

        saveBtn.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View V) {
                String polish = polishWord.getText().toString();
                String english = translationWord.getText().toString();
                String pronunc = pronunciation.getText().toString();


                if (new String("").equals(polish) || new String("").equals(english) || new String("").equals(pronunc))
                {
                    Toast.makeText(getApplicationContext(), "Fill all fields", Toast.LENGTH_SHORT).show();
                    return;
                }

                DatabaseHandler db = new DatabaseHandler(NewPolishWordActivity.this);
                db.insertTranslation(polish, english, pronunc);

                Toast.makeText(getApplicationContext(), "Word Inserted Successfully", Toast.LENGTH_SHORT).show();

                finish();
            }
        });
    }

}




































