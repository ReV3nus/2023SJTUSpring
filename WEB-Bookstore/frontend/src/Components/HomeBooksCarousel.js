import React from 'react';
import Carousel1 from '../Assets/carousel/book1.jpg'
import Carousel2 from '../Assets/carousel/book2.jpg'
import Carousel3 from '../Assets/carousel/book3.jpg'
import Carousel4 from '../Assets/carousel/book4.jpg'
import '../CSS/HomeCSS.css'

import {Carousel} from "antd";

class HomeBooksCarousel extends React.Component{

    render(){
        return (
            <div id="Carousel">
                <Carousel autoplay style={{
                    width:'600px',
                }}>
                    <div>
                        <img alt="Carousel1" src={Carousel1} id='CarouselImages'/>
                    </div>
                    <div>
                        <img alt="Carousel2" src={Carousel2} id='CarouselImages'/>
                    </div>
                    <div>
                        <img alt="Carousel3" src={Carousel3} id='CarouselImages'/>
                    </div>
                    <div>
                        <img alt="Carousel4" src={Carousel4} id='CarouselImages'/>
                    </div>
                </Carousel>
            </div>
        )
    }
}
export default HomeBooksCarousel;


