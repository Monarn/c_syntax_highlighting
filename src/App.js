import logo from './logo.svg';
import './App.css';
import { useEffect, useState } from 'react';
import axios from 'axios';

function App() {
  const [text, setText] = useState('')
  const [coloratedText, setColoratedText] = useState("")
  const handleText = (event) => {
    setText(event.target.value.replace(/"/g, '\\"'));
  }

  useEffect(() => {
    const coloration = async () => {
      axios.get(`http://localhost:3001/parse?code=${encodeURIComponent(text)}`).then(res => {
        setColoratedText(res.data);
      })
    }
    coloration()
  }, [text])

  return (
    <div className="App">
      <textarea onChange={handleText}></textarea>
      <pre>
        <div className='code' dangerouslySetInnerHTML={{__html: coloratedText}}></div>
      </pre>
    </div>
  );
}

export default App;
