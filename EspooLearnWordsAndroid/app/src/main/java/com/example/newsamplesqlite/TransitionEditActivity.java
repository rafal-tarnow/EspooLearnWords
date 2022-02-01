package com.example.newsamplesqlite;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class TransitionEditActivity extends AppCompatActivity {
    private EditText englishWordEditText;
    private EditText pronunciationEditText;
    private EditText foreignEditText;
    private Intent intent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_transition_edit);

        englishWordEditText = (EditText) findViewById(R.id.englishWord);
        pronunciationEditText = (EditText) findViewById(R.id.pronunciationWord);
        foreignEditText = (EditText) findViewById(R.id.foreignWord);

        Button update_button = (Button) findViewById(R.id.update_button);
        Button delete_button = (Button) findViewById(R.id.delete_button);

        intent = getIntent();
        String nameExtra = intent.getStringExtra(SampleMaterialActivity.EXTRA_ENGLISH_WORD);
        String pronunExtra = intent.getStringExtra(SampleMaterialActivity.EXTRA_PRONUN);
        String foreign = intent.getStringExtra(SampleMaterialActivity.EXTRA_FOREIGN);

        String initialExtra = intent.getStringExtra(SampleMaterialActivity.EXTRA_INITIAL);
        int colorExtra = intent.getIntExtra(SampleMaterialActivity.EXTRA_COLOR, 0);

        englishWordEditText.setText(nameExtra);
        englishWordEditText.setSelection(englishWordEditText.getText().length());

        pronunciationEditText.setText(pronunExtra);
        foreignEditText.setText(foreign);
//        initialTextView.setText(initialExtra);
//        initialTextView.setBackgroundColor(colorExtra);

        englishWordEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (s.length() == 0) {
                    // Aktualizujemy kontrolkę initialTextView.
                    //initialTextView.setText("");
                } else if (s.length() >= 1) {
                    // W kontrolce initialTextView zapisujemy pierwszą literę z kontrolki nameEditText i aktualizujemy
                    // informacje dodatkowe w intencji.
                    //initialTextView.setText(String.valueOf(s));
                    intent.putExtra(SampleMaterialActivity.EXTRA_UPDATE, true);
                }
            }

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
            }

            @Override
            public void afterTextChanged(Editable s) {
            }
        });

        update_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Musi być różne od zera, w przeciwnym razie nie kończymy aktywności i wyświetlamy komunikat Toast.
                String text = String.valueOf(englishWordEditText.getText()).trim();
                if (TextUtils.isEmpty(text)) {
                    Toast.makeText(getApplicationContext(), "Podaj słówko", Toast.LENGTH_SHORT).show();
                } else {
                    intent.putExtra(SampleMaterialActivity.EXTRA_UPDATE, true);
                    intent.putExtra(SampleMaterialActivity.EXTRA_ENGLISH_WORD, String.valueOf(englishWordEditText.getText()));
                    intent.putExtra(SampleMaterialActivity.EXTRA_PRONUN, String.valueOf(pronunciationEditText.getText()));
                    intent.putExtra(SampleMaterialActivity.EXTRA_FOREIGN, String.valueOf(foreignEditText.getText()));
                    intent.putExtra(SampleMaterialActivity.EXTRA_INITIAL, englishWordEditText.getText());

                    setResult(RESULT_OK, intent);
                    supportFinishAfterTransition();
                }
            }
        });

        delete_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                intent.putExtra(SampleMaterialActivity.EXTRA_DELETE, true);

                setResult(RESULT_OK, intent);
                supportFinishAfterTransition();
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == android.R.id.home) {
            super.onBackPressed();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}

